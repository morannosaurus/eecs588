# A Toy Rootkit #

This repository contains the code for an Ubuntu loadable kernel module. This module can be used to hide processes, files, and directories from userland processes. This was created as a project in EECS 588 (Computer Security) at the University of Michigan. Steps have been taken to make this module difficult to use in a real attack - for example, this is likely to work *only* on the particular kernel we targeted (Ubuntu 14.04 LTS); it doesn't hide any network activity, making it less useful for most malicious programs; and it is trivially easy to remove if its presence is known.

To activate the rootkit, run the following commands:
 * `make`
 * `sudo ./deploy.sh`
Then, reboot the machine. You'll see that `/var/log/foobar` is having `foobar` appended to it once a second, but the process performing this action will be invisible.

To deactivate the rootkit, run `./deactivate`. To remove the rootkit, run `sudo ./undeploy.sh`.

The presentation can be found at https://github.com/IanPudney/eecs588/raw/master/588_attack_presentation.pdf.
