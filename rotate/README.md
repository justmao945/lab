# Rotate

Rotate output of application to log files.

## Installation
```sh
go get github.com/justmao945/rotate
```

## Usage

```sh
my_app_print 2>&1 | rotate -name my_app -size=32 -dir my_log_dir
```
