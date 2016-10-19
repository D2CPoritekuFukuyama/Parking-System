require "open3"

stdin, stdout, stderr = Open3.capture3('./main')
stdArray = stdin.split("\n")
puts stdArray[0]
