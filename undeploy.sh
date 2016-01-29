#Uninstalls the kernel module!

export SECRET_KO_NAME=$(cat secret_ko_name)
export SECRET_PAYLOAD_NAME=$(cat secret_payload_name)

# deactivate the module
./deactivate

# remove the kernel module from the drivers directory
rm "/lib/modules/$(uname -r)/kernel/drivers/$SECRET_KO_NAME.ko"
# remove this module as a startup module
sed "/$SECRET_KO_NAME/d" /etc/modules > modulebak
cp -f modulebak /etc/modules
# register these changes with the OS
depmod

# remove the payload from the /usr/bin directory
rm /usr/bin/$SECRET_PAYLOAD_NAME
