#!/bin/bash

GPU=$(lspci | grep -i '.* vga .* nvidia .*')
IMAGE_NAME="kakalin/qt:5.12.0_opencv_gstreamer"

docker_run_params=$(cat <<-END
    --rm \
    -it \
    -e DISPLAY=$DISPLAY \
    -e QT_X11_NO_MITSHM=1 \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    -v $PWD:/home/user/YUVPlayer \
    --device /dev/dri \
    --device /dev/snd \
    --network=host \
    -v $HOME/.Xauthority:/root/.Xauthority \
    -e PULSE_SERVER=unix:${XDG_RUNTIME_DIR}/pulse/native \
    -v ${XDG_RUNTIME_DIR}/pulse/native:${XDG_RUNTIME_DIR}/pulse/native \
    -w /home/user/YUVPlayer \
    $IMAGE_NAME
END
)

xhost +local:docker
xhost +localhost
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    if [[ $GPU == *' NVIDIA '* ]]; then
        # printf 'Nvidia GPU is present:  %s\n' "$GPU"
        docker run \
            --gpus all \
            $docker_run_params
    else
        docker run \
            $docker_run_params
    fi
elif [[ "$OSTYPE" == "darwin"* ]]; then
    DISPLAY=host.docker.internal:0
    docker run \
        -e QT_QUICK_BACKEND=software \
        $docker_run_params
else
    echo "Haven't supported this OS Type, please check command\n and update it."
fi
