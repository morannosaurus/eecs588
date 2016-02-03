#Uninstalls the kernel module!


# deactivate the module and stop the process
./deactivate
pkill -f "$(cat secret_payload_name)"
echo $?

# remove the module
rm "/usr/bin/$(cat secret_ko_name).ko"
echo $?

# remove the payload
rm "/usr/bin/$(cat secret_payload_name)"
echo $?

# remove the startup configuration
rm "/etc/init/$(cat secret_conf_name).conf"
echo $?

