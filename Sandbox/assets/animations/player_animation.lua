player_animation = {
    idle = function (duration) 
        return 0
    end,
    
    forward = function (duration) 
        duration = duration % 2
        return math.floor(0 + 2 * duration);
    end,
    
    back = function (duration) 
        duration = duration % 2
        return math.floor(4 + 2 * duration)
    end,
    
    left = function (duration) 
        duration = duration % 2
        return math.floor(8 + 2 * duration)
    end,
    
    right = function (duration) 
        duration = duration % 2
        return math.floor(12 + 2 * duration)
    end,
    
    left_back = function (duration) 
        duration = duration % 2
        return math.floor(16 + 2 * duration)
    end,
    
    right_back = function (duration) 
        duration = duration % 2
        return math.floor(20 + 2 * duration)
    end,
    
    left_forward = function (duration) 
        duration = duration % 2
        return math.floor(24 + 2 * duration)
    end,
    
    right_forward = function (duration) 
        duration = duration % 2
        return math.floor(28 + 2 * duration)
    end
}
