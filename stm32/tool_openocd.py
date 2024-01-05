Import("env")

# Explicitly specify the path to openocd
env.Replace(
    UPLOADER="openocd",
    UPLOADERFLAGS=[
        "-d2",
        "-s", "$PACKAGE_DIR/openocd/scripts",
        "-f", "interface/stlink.cfg",
        "-c", "transport select hla_swd",
        "-f", "target/stm32wlx.cfg",
        "-c", "program {$SOURCE} verify reset; shutdown;"
    ],
    UPLOADCMD="$UPLOADER $UPLOADERFLAGS"
)