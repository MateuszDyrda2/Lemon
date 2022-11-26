player2_animation = {
    idle = function (duration)
        return 4
    end,

    forward = function (duration)
        duration = duration % 1
        if duration < 0.25 then return 3
        elseif duration < 0.5 then return 4
        elseif duration < 0.75 then return 5
        else return 4 end
    end,

    back = function (duration)
        duration = duration % 1
        if duration < 0.25 then return 9
        elseif duration < 0.5 then return 10
        elseif duration < 0.75 then return 11
        else return 10 end
    end,

    right = function (duration)
        duration = duration % 1
        if duration < 0.25 then return 6
        elseif duration < 0.5 then return 7
        elseif duration < 0.75 then return 8
        else return 7 end
    end,

    left = function (duration)
        duration = duration % 1
        if duration < 0.25 then return 0
        elseif duration < 0.5 then return 1
        elseif duration < 0.75 then return 2
        else return 1 end
    end
}
