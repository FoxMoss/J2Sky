# J2Sky
Bluesky for J2ME phones.

Inspired by [celeste-j2me](https://github.com/r58Playz/celeste-j2me).

## FAQ

### Why J2ME?
It's fun? Also wanted a chance to get an old [flip phone](https://retrospect.hackclub.com/j2me).

### Why did you write this in C?
Compiling to MIPS then to Java bytecode just seemed easier then learning Java.

### How to install?

First install the proper Java versions and Wireless Toolkit from [setting-up-j2me-in-2025-debian](https://github.com/FoxMoss/Setting-Up-j2me-in-2025-debian).
Create a Debian Squeeze (6) chroot with java and python2 in it to run Cibyl. I did this like so:
```bash
sudo debootstrap --arch i386 squeeze /srv/debian6 http://archive.debian.org/debian/
```
Then to enter your Debian 6 install, mount your systems onto the install and chroot in.
```bash
sudo mount -o bind /dev /srv/debian6/dev
sudo mount -o bind /proc /srv/debian6/proc
sudo mount -o bind /sys /srv/debian6/sys
```
```bash
sudo chroot /srv/debian6/ bash
```
Install [Cibyl](https://github.com/SimonKagstrom/cibyl) on Debian 6.
Run the build scripts and create any directories as needed.

If you're struggling feel free to contact me, the setup is very esoteric.
