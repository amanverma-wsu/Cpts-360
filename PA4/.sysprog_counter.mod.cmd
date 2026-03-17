savedcmd_sysprog_counter.mod := printf '%s\n'   sysprog_counter.o | awk '!x[$$0]++ { print("./"$$0) }' > sysprog_counter.mod
