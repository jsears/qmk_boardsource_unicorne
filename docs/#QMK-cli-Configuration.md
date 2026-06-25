#QMK-cli Configuration

git config --global user.name "Jenn Sears"

QMK CLI installation complete.
The QMK CLI has been installed to '/opt/uv/tools/bin'.
The QMK CLI venv has been created at '/opt/uv/tools/qmk'.
Toolchains and flashing utilities have been installed to '/opt/qmk'.

You may need to restart your shell to gain access to the 'qmk' command.
Alternatively, add /opt/uv/tools/bin to your $PATH:
    export PATH="/opt/uv/tools/bin:$PATH"

qmk config user.overlay_dir="$(realpath qmk_boardsource_unicorne)"
