#! /bin/sh
while : ; do
  mk-kill -uroot --iterations 0 --interval 1 --busy-time 10 --match-info ^SELECT \
    --print --kill --no-only-oldest \
  | tee mk-kill.out \
  | cut -c1-110
  sleep 1
done
