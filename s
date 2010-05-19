#! /bin/sh
SOCK_DIR=/tmp/slumos
[ -d "$SOCK_DIR" ] || mkdir -m 0700 "$SOCK_DIR"
[ -n "$SSH_AUTH_SOCK" ] && ln -nfs "$SSH_AUTH_SOCK" "$SOCK_DIR/ssh.auth.sock"
screen -xRR 

