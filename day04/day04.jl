using Dates: Date, Time, daysinyear, dayofyear, hour, minute, year

YEAR = 1518

function parse_record(record::AbstractString)
    regex = r"\[(\d\d\d\d)-(\d\d)-(\d\d) (\d\d):(\d\d)\] (Guard #(\d+) )?\s*(.*)\s*$"
    m = match(regex, record)
    if m === nothing
        throw(ArgumentError("Not a valid record: $record"))
    end
    date = Date(parse(Int, m.captures[1]),
                parse(Int, m.captures[2]),
                parse(Int, m.captures[3]))
    if year(date) ≠ 1518
        throw(ArgumentError("Unexpected year: $(year(date))"))
    end
    time = Time(parse(Int, m.captures[4]),
                parse(Int, m.captures[5]))
    id = m.captures[6]
    if id !== nothing
        id = parse(Int, m.captures[7])
    end
    return dayofyear(date), time, id, m.captures[8]
end


function check_hour(time)
    h = hour(time)
    if h != 0 throw(ArgumentError("Unexpected hour: $h")) end
end


function main()
    # onduty[i] is the ID of the guard on duty on the i-th day of the year
    onduty = -ones(Int, daysinyear(YEAR))
    # asleep[i, j] = 1 if the guard was asleep during the j-th minute of the
    # i-th day of the year
    asleep = zeros(Int, daysinyear(YEAR), 60)

    open("input.txt", "r") do f
        for line in eachline(f)
            day, time, id, event = parse_record(line)
            if event == "begins shift"
                # If shift starts at between 23h and 00h, it must be
                # assigned to the following day!!!
                if hour(time) == 23
                    onduty[day+1] = id
                elseif hour(time) == 0
                    onduty[day] = id
                else
                    throw(ArgumentError("Unexpected time: $time"))
                end
            elseif event == "falls asleep"
                check_hour(time)
                asleep[day, minute(time)+1:60] .+= 1
            elseif event == "wakes up"
                check_hour(time)
                asleep[day, minute(time)+1:60] .-= 1
            else
                throw(ArgumentError("Unexpected event: $event"))
            end
        end
    end

    sleep_length_by_id = Dict{Int, Int}()
    for (i, id) in enumerate(onduty)
        if id ≠ -1
            sleep_length_by_id[id] = (get(sleep_length_by_id, id, 0)
                                      +sum(asleep[i, :]))
        end
    end

    preferred_guard = -1
    minutes_asleep = -1
    for (key, value) in sleep_length_by_id
        if value > minutes_asleep
            preferred_guard = key
            minutes_asleep = value
        end
    end
    println("Guard $preferred_guard spent $minutes_asleep minutes asleep")

    hist = zeros(Int, 60)
    for (i, id) in enumerate(onduty)
        if id == preferred_guard
            hist .+= asleep[i, :]
        end
    end

    preferred_minute = argmax(hist)-1
    println("Guard $preferred_guard slept most during minute $preferred_minute")
    println("The answer for strategy 1 is: $(preferred_guard*preferred_minute)")

    asleep_by_id = Dict()
    for (i, id) in enumerate(onduty)
        if id ≠ -1
            asleep_by_id[id] = (get(asleep_by_id, id, zeros(Int, 60))
                                .+asleep[i, :])
        end
    end

    preferred_guard = -1
    preferred_minute = -1
    max_freq = -1

    for (key, value) in asleep_by_id
        test = maximum(value)
        if test > max_freq
            max_freq = test
            preferred_minute = argmax(value)-1
            preferred_guard = key
        end
    end

    println("Guard $preferred_guard slept $(max_freq) times on minute $preferred_minute")
    println("The answer for strategy 2 is: $(preferred_guard*preferred_minute)")
end

main()
