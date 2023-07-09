Disable `ptrace_scope` so that we won't be asked to authenticate when attaching to a process.
```bash
echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope
```