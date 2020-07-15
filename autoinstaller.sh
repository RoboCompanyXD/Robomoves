#!/bin/bash

# Instalar OpenCV 4.2.0

OPENCV_VERSION='4.2.0'

# Actualizar

sudo apt-get -y update

# Comprobar estádo de la cámara

camstatuss=$(vcgencmd get_camera)

if [[ $camstatuss =~ "supported=0" ]]; then 
    echo "La cámara no está habilitada."; 
    echo "A continuación se abrirá la configuracion de la Raspberry Pi"; 
    echo "Habilite el puerto de la cámara: nterfacing Options > Camera"; 
    read -n 1 -s -r -p "Pulsar una tecla para abir configuración de raspberry";
    sudo raspi-config; exit 1;
fi

if [[ $camstatuss =~ "detected=0" ]]; then
    echo "No se ha detectado ninguna cámara.";
    echo "Compruebe las conexiones.";
    echo "Presione a para salir o cualquier tecla para continuar.";
    read -rsn1 input; if [ "$input" = "a" ]; then
        echo "Abortando..."; exit 1;
    fi;
fi

# Instalar developer tools

sudo apt-get install -y build-essential cmake pkg-config

# Instalar paquetes de E/S de imagen

sudo apt-get install -y qt5-default libvtk6-dev

# Instalar paquetes de E/S de video

sudo apt-get install -y libdc1394-22-dev libavcodec-dev libavformat-dev libswscale-dev \

                        libtheora-dev libvorbis-dev libxvidcore-dev libx264-dev yasm \

                        libopencore-amrnb-dev libopencore-amrwb-dev libv4l-dev libxine2-dev


sudo apt-get install -y zlib1g-dev libjpeg-dev libwebp-dev libpng-dev libtiff5-dev libjasper-dev \

                        libopenexr-dev libgdal-dev

# Para compilar Highgui

sudo apt-get install -y libfontconfig1-dev libcairo2-dev

# Librerias para optimizacion

sudo apt-get install -y libgdk-pixbuf2.0-dev libpango1.0-dev
sudo apt-get install -y libgtk2.0-dev libgtk-3-dev
sudo apt-get install -y libtbb-dev libeigen3-dev

sudo apt-get install -y libatlas-base-dev gfortran

# Para HDF5 y GUI Qt

sudo apt-get install -y libhdf5-dev libhdf5-serial-dev libhdf5-103
sudo apt-get install -y libqtgui4 libqtwebkit4 libqt4-test python3-pyqt5

# Instalar Python

sudo apt-get install -y python-dev  python-tk  pylint  python-numpy  \

                        python3-dev python3-tk pylint3 python3-numpy flake8

sudo apt-get install -y qt5-default

wget https://bootstrap.pypa.io/get-pip.py

sudo python get-pip.py
sudo python3 get-pip.py
sudo rm -rf ~/.cache/pip

# Preparar virtual enviroment

sudo echo "# virtualenv and virtualenvwrapper" >> ~/.bashrc
sudo echo "export WORKON_HOME=$HOME/.virtualenvs" >> ~/.bashrc
sudo echo "export VIRTUALENVWRAPPER_PYTHON=/usr/bin/python3" >> ~/.bashrc
sudo echo "source /usr/local/bin/virtualenvwrapper.sh" >> ~/.bashrc

source ~/.bashrc

sudo pip install virtualenv virtualenvwrapper

pip install "picamera[array]"

pip install numpy

sudo apt-get install -y ant default-jdk

sudo apt-get install -y doxygen unzip wget

# Descargar y desempaquetar OpenCV y OpenCV-Contrib

wget https://github.com/opencv/opencv/archive/${OPENCV_VERSION}.zip

unzip ${OPENCV_VERSION}.zip && rm ${OPENCV_VERSION}.zip

mv opencv-${OPENCV_VERSION} OpenCV

wget https://github.com/opencv/opencv_contrib/archive/${OPENCV_VERSION}.zip

unzip ${OPENCV_VERSION}.zip && rm ${OPENCV_VERSION}.zip

mv opencv_contrib-${OPENCV_VERSION} opencv_contrib

mv opencv_contrib OpenCV

cd OpenCV && mkdir build && cd build

cmake \
-D WITH_OPENGL=ON \
-D FORCE_VTK=ON \
-D WITH_TBB=ON \
-D WITH_GDAL=ON \
-D ENABLE_NEON=ON \
-D ENABLE_VFPV3=ON \
-D WITH_XINE=ON \
-D ENABLE_PRECOMPILED_HEADERS=OFF \
-D PYTHON_EXECUTABLE=/usr/bin/python3.7 \
-D PYTHON_DEFAULT_EXECUTABLE=/usr/bin/python3.7 \
-D PYTHON3_NUMPY_INCLUDE_DIRS:PATH=/usr/lib/python3/dist-packages/numpy/core/include \
-D OPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules \
-D BUILD_TESTS=OFF \
-D INSTALL_PYTHON_EXAMPLES=ON \
-D OPENCV_ENABLE_NONFREE=ON \
-D CMAKE_SHARED_LINKER_FLAGS=-latomic \
-D OPENCV_GENERATE_PKGCONFIG=YES \
-D CMAKE_INSTALL_PREFIX=/usr/local \
..

echo "Comprueba que está todo correcto, pulsa a para abortar o cualquier tecla para continuar: "

read -rsn1 input
if [ "$input" = "a" ]; then
    echo "Abortando..."
    exit 1
fi

make -j6

sudo make install

sudo ldconfig

# Instalar Bluez

BLUEZ_VERSION='5.53'

sudo apt-get install -y libusb-dev libdbus-1-dev libglib2.0-dev libudev-dev libical-dev libreadline-dev

wget http://www.kernel.org/pub/linux/bluetooth/bluez-${BLUEZ_VERSION}.tar.xz
tar xvf bluez-${BLUEZ_VERSION}.tar.xz && rm bluez-${BLUEZ_VERSION}.tar.xz

mv bluez-${BLUEZ_VERSION} bluez

cd bluez

./configure --enable-library

make -j6

sudo make install

sudo sed -i "s@ExecStart=/usr/local/libexec/bluetooth/bluetoothd@ExecStart=/usr/local/libexec/bluetooth/bluetoothd --experimental --noplugin=sap@" /lib/systemd/system/bluetooth.service

pip3 install pybluez
pip install pybluez

sudo systemctl daemon-reload
sudo service bluetooth restart

systemctl status bluetooth

# Instalar libreria de bitalino

pip3 install bitalino
pip install bitalino

# Instalar libreria PyLidar3

pip3 install PyLidar3

# Instalar NoIp

wget https://www.noip.com/client/linux/noip-duc-linux.tar.gz
tar vzxf noip-duc-linux.tar.gz
rm noip-duc-linux.tar.gz
mv noip-* noip
cd noip
sudo make

echo 'noconfiginstall: ${TGT}' >> ./Makefile
echo -e '\tif [ ! -d ${BINDIR} ]; then mkdir -p ${BINDIR};fi' >> ./Makefile
echo -e '\tif [ ! -d ${CONFDIR} ]; then mkdir -p ${CONFDIR};fi' >> ./Makefile
echo -e '\tcp ${TGT} ${BINDIR}/${TGT}' >> ./Makefile

sudo make noconfiginstall
sudo echo "port 22" >> /etc/ssh/sshd_config
sudo echo "port 12366" >> /etc/ssh/sshd_config
sudo systemctl restart ssh

# Añadir reglas para dispositivos USB

echo -e 'ACTION=="add", ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6015", ATTRS{serial}=="DN04GZXQ", SYMLINK+="ttyUSBRobot"' >> /etc/udev/rules.d/10-local.rules && echo -e 'ACTION=="add", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="ea60", ATTRS{serial}=="0001", SYMLINK+="ttyUSBLidar"' >> /etc/udev/rules.d/10-local.rules

# Habilitar SSH

ssh-keygen -A && update-rc.d ssh enable && invoke-rc.d ssh start

# Crenerar fichero ~/.ssh/authorized_keys

mkdir ~/.ssh
chmod 0700 ~/.ssh
touch ~/.ssh/authorized_keys
chmod 0644 ~/.ssh/authorized_keys

# TODO: Clonar respositorios necesarios

# git clone ...
