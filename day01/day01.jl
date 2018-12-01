open("input.txt", "r") do f
    global changes = [parse(Int, line) for line in eachline(f)]
end

println("Final frequency = $(sum(changes))")

unique_ = Set{Int}()
freq = 0
for change in Iterators.cycle(changes)
    global freq += change
    if in(freq, unique_)
        println("First frequency reached twice = $freq")
        break
    else
        push!(unique_, freq)
    end
end
