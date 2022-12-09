#!/bin/bash

IMAGE_NAME="kakalin/qt:5.12.0_opencv_gstreamer"
docker_run_params=$(cat <<-END
    --rm -it \
    -e QT_X11_NO_MITSHM=1 \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    -v $PWD:/home/user/YUVPlayer \
    --network=host \
    -v $HOME/.Xauthority:/root/.Xauthority \
    -w /home/user/YUVPlayer \
    --privileged \
    $IMAGE_NAME
END
)

xhost +localhost
xhost +local:docker
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    GPU=$(lspci | grep -i '.* vga .* nvidia .*')
    if [[ $GPU == *' NVIDIA '* ]]; then
        # printf 'Nvidia GPU is present:  %s\n' "$GPU"
        docker run \
            --gpus all \
            -e DISPLAY=$DISPLAY \
            -e PULSE_SERVER=unix:${XDG_RUNTIME_DIR}/pulse/native \
            -v ${XDG_RUNTIME_DIR}/pulse/native:${XDG_RUNTIME_DIR}/pulse/native \
            $docker_run_params
    else
        docker run \
            -e DISPLAY=$DISPLAY \
            -e QT_QUICK_BACKEND=software \
            -e PULSE_SERVER=unix:${XDG_RUNTIME_DIR}/pulse/native \
            -v ${XDG_RUNTIME_DIR}/pulse/native:${XDG_RUNTIME_DIR}/pulse/native \
            $docker_run_params
    fi
elif [[ "$OSTYPE" == "darwin"* ]]; then
    docker run \
        -e DISPLAY=host.docker.internal:0 \
        -e QT_QUICK_BACKEND=software \
        $docker_run_params
else
    echo "Haven't supported this OS Type, please check command\n and update it."
fi
