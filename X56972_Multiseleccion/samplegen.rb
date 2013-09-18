# Max number that rand can generate
MAX = 200
SAMPLES = 5

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

# Do 50 times (50 arrays)
SAMPLES.times do |t|
  elements = (t+1) * SIZE_JUMP

  # t is the current iteration, 0 <= t < 50
  # Outputs a message of the current operation
	puts "Generating #{t+1} vector of #{elements} elements..."

  # Generates an array of (t+1) * SIZE_JUMP elements
	v = rand_array(elements)
  p = [ 1 ]
  
  seed = rand(48) + 2

  elements.times do |i|
    next if i < 1

    p << i+1 if rand(seed) == 0
  end

  # Creates a new file in vectors/vector_|t|.dat with write permissions
	File.open("samples_big/sample_#{format('%02d', t+1)}.dat", "w") do |f|
    # f is the created file
		# Joins each element of the array with a line break and writes the result
    # in the created file 
    f.write("#{elements} #{p.size} #{p.join(" ")} #{v.join(" ")}")
	end
end
