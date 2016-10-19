require "open3"

stdin, stdout, stderr = *Open3.popen3('./main')
puts stdin
