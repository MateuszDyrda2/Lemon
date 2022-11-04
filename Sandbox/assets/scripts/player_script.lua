player_script = {}

print("hello from lua")

player_script.idle = function (duration) 
    return 0
end

player_script.forward = function (duration) 
    duration = duration % 2
    return math.floor(0 + 2 * duration);
end

player_script.back = function (duration) 
    duration = duration % 2
    return math.floor(4 + 2 * duration)
end

player_script.left = function (duration) 
    duration = duration % 2
    return math.floor(8 + 2 * duration)
end

player_script.right = function (duration) 
    duration = duration % 2
    return math.floor(12 + 2 * duration)
end

player_script.left_back = function (duration) 
    duration = duration % 2
    return math.floor(16 + 2 * duration)
end

player_script.right_back = function (duration) 
    duration = duration % 2
    return math.floor(20 + 2 * duration)
end

player_script.left_forward = function (duration) 
    duration = duration % 2
    return math.floor(24 + 2 * duration)
end

player_script.right_forward = function (duration) 
    duration = duration % 2
    return math.floor(28 + 2 * duration)
end
