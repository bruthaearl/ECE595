qemu-system-arm \
-M versatilepb \
-kernel buildroot/output/images/zImage \
-dtb buildroot/output/images/versatile-pb.dtb \
-drive file=buildroot/output/images/rootfs.ext2,if=scsi,format=raw -append "root=/dev/sda console=ttyAMA0,115200" \
-serial stdio \
-netdev user,id=ether0,hostfwd=tcp:127.0.0.1:2222-:22 \
-device rtl8139,netdev=ether0 \
