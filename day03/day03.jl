function parse_claim(claim::AbstractString)
    regex = r"#(\d*) @ (\d*),(\d*): (\d*)x(\d*)"
    m = match(regex, claim)
    if m === nothing
        throw(ArgumentError("Not a valid claim: $claim"))
    end
    return map(s -> parse(Int, s), m.captures)
end


function main()
    lines = []
    open("input.txt", "r") do f
        lines = [line for line in eachline(f)]
    end
    num_claims = length(lines)
    println("There are $num_claims claims to parse.")
    claims = Array{Int}(undef, num_claims, 5)
    for (i, line) in enumerate(lines)
        claims[i, :] = parse_claim(line)
    end
    fabric_width = maximum(claims[:, CLAIM_LEFT]+claims[:, CLAIM_WIDTH])
    fabric_height = maximum(claims[:, CLAIM_TOP]+claims[:, CLAIM_HEIGHT])
    println("Size of fabric: $fabric_width × $fabric_height.")
    fabric = zeros(Int, fabric_width, fabric_height)
    for i in 1:num_claims
        id, left, top, width, height = claims[i, :]
        fabric[left+1:left+width, top+1:top+height] .+= 1
    end
    overlaps = sum(fabric .≥ 2)
    println("$overlaps square inches of fabric are within two or more claims.")

    for i in 1:num_claims
        id, left, top, width, height = claims[i, :]
        if maximum(fabric[left+1:left+width, top+1:top+height]) == 1
            print("Claim $id has no overlap.")
        end
    end
end

main()

# There are 1357 claims to parse.
# Size of fabric: 1000 × 1000.
# 113716 square inches of fabric are within two or more claims.
# Claim 742 has no overlap.
