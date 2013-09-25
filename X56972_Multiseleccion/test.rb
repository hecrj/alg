require 'fileutils'
require 'colorize'

# Max number that rand can generate
MAX = 10**6
SAMPLES = 50

# Number of elements to add to next vector
SIZE_JUMP = (10**6) / SAMPLES

# Generates a random array of size elements
def rand_array(size, max = MAX)
  # New array of size elements
  numbers = Array.new(size)

  # Fill array with random elements
  numbers.map! { rand(max) }

  # Return generated array
  numbers
end

class Test
  def initialize(size)
    @size = size
  end

  def generate
    # Generates an array of (t+1) * SIZE_JUMP @size
    v = rand_array(@size)
    p = [ 1 ]
    
    seed = rand(48) + 2

    @size.times do |i|
      next if i < 1

      p << i+1 if rand(seed) == 0
    end

    # Creates a new file in vectors/vector_|t|.dat with write permissions
    File.open("test_#{@size}.dat", "w") do |f|
      # f is the created file
      # Joins each element of the array with a line break and writes the result
      # in the created file 
      f.write("#{@size} #{p.size} #{p.join(" ")} #{v.join(" ")}")
    end

    v.sort!

    File.open("test_#{@size}.out", "w") do |f|
      f.write("#{p.map { |i| v[i-1] }.join(" ")}\n")
    end
  end

  def perform
    print "Testing #{@size} elements... ".yellow

    generate

    beginning_time = Time.now
    execution = system("./test < test_#{@size}.dat > test_#{@size}_exec.out")
    end_time = Time.now

    unless execution
      print "Failed".orange
      return
    end

    if FileUtils.cmp("test_#{@size}.out", "test_#{@size}_exec.out")
      print "Passed".green
      print " "
      print "#{(end_time - beginning_time)*1000} ms".blue
    else
      print "Wrong".red
      return
    end

    File.delete("test_#{@size}.dat")
    File.delete("test_#{@size}.out")
    File.delete("test_#{@size}_exec.out")

    puts ""
  end

end

puts "Compiling main.cc...".blue
#compiled = system("g++ main.cc -o test -O2")

unless compiled
  puts "Compilation failed.".red
  exit(1)
end

SAMPLES.times do |t|
  size = (t+1) * SIZE_JUMP
  test = Test.new(size)
  test.perform
end

File.delete("test")
