#!/bin/bash

# Read parent pid, child pid
print_path=0
while [ "$#" -gt 0 ]; do
  case "$1" in
    --parent)
      parent="$2"
      shift 2
      ;;
    --child)
      child="$2"
      shift 2
      ;;
    --path)
      print_path=1
      shift 1
      ;;
    *)
      echo "Unknown parameter passed: $1"
      exit 1
      ;;
  esac
done

# Check if parent or child is empty
if [ -z "$parent" ] || [ -z "$child" ]; then
  echo "Usage: $0 --parent PARENT_PID --child CHILD_PID [--path]"
  exit 1
fi

########
# TODO #
########

found=0
length=0
while [ "$child" -ne 1 ]; do
  relation["$length"]="$child"
  child=$(ps -o ppid= -p "$child" | awk '{print $1}')
  length=$((length+1))
  if [ "$parent" -eq "$child" ]; then
    echo Yes
    if [ "$print_path" -eq 1 ]; then
      echo -n "$parent"
      for i in "${relation[@]}"; do
        echo -n " -> $i"
      done
    fi
    found=1
    break
  fi
done

if [ "$found" -ne 1 ]; then
  echo No
fi
# The code below is only for demonstration purposes, please remove it before submitting.
