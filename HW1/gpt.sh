#!/bin/bash

# Function to check if a process is an ancestor of another
is_ancestor() {
  local child_pid="$1"
  local potential_ancestor_pid="$2"

  # Check if the child_pid equals potential_ancestor_pid
  if [ "$child_pid" -eq "$potential_ancestor_pid" ]; then
    return 0  # The child is the same as the potential ancestor
  fi

  # Iterate through the parent processes of the child
  local parent_pid="$child_pid"
  while [ "$parent_pid" -ne 1 ]; do
    parent_pid=$(ps -o ppid= -p "$parent_pid" | awk '{print $1}')
    if [ "$parent_pid" -eq "$potential_ancestor_pid" ]; then
      return 0  # Found a match, potential_ancestor_pid is an ancestor of child_pid
    fi
  done

  return 1  # No match found, not an ancestor
}

# Usage: ./is_ancestor.sh <child_pid> <potential_ancestor_pid>
if [ "$#" -ne 2 ]; then
  echo "Usage: $0 <child_pid> <potential_ancestor_pid>"
  exit 1
fi

child_pid="$1"
potential_ancestor_pid="$2"

if is_ancestor "$child_pid" "$potential_ancestor_pid"; then
  echo "$potential_ancestor_pid is an ancestor of $child_pid."
else
  echo "$potential_ancestor_pid is not an ancestor of $child_pid."
fi
