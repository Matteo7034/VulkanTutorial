for f in Makefile include/*.h include/*.hpp src/*.cpp utils/*.cpp; do
  if [ -f "$f" ]; then
    echo "=== FILE: $f ==="
    cat "$f"
    echo -e "\n"
  fi
done > /tmp/vulkan.txt
