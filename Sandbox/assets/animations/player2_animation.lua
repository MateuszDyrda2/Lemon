player2_animation = {
    idle = function (duration)
        return 7
    end,

    forward = function (duration)
        duration = duration % 1.5
        if duration < 0.5 then return 6
        elseif duration < 1.0 then return 7
        else return 8 end
    end

    back = function (duration)
        duration = duration % 1.5
        if duration < 0.5 then return 0
        elseif duration < 1.0 then return 1
        else return 2 end
    end

    right = function (duration)
        duration = duration % 1.5
        if duration < 0.5 then return 3
        elseif duration < 1.0 then return 4
        else return 5 end
    end

    left = function (duration)
        duration = duration % 1.5
        if duration < 0.5 then return 9
        elseif duration < 1.0 then return 10
        else return 11 end
    end
}
