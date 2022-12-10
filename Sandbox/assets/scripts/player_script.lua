player_script = { }

local speed = 300

local IDLE = 0
local RIGHT = 1
local LEFT = 2
local FORWARD = 3
local BACK = 4
local currentDirection = IDLE 

local function animation_frame (ent, x, y) 
    if x > 0 then 
        if currentDirection ~= RIGHT then
            currentDirection = RIGHT 
            ent.animator:start("right")
        end
    elseif x < 0 then 
        if currentDirection ~= LEFT then
            currentDirection = LEFT
            ent.animator:start("left")
        end
    elseif y > 0 then 
        if currentDirection ~= BACK then
            currentDirection = BACK 
            ent.animator:start("back")
        end
    else 
        if currentDirection ~= FORWARD then
            currentDirection = FORWARD 
            ent.animator:start("forward")
        end
    end
end

function player_script.on_physics (ent, delta) 
    x, y = input:check_axis(65, 68), input:check_axis(83, 87)
    position = lemon.get_transform(ent).position

    length = math.sqrt(x * x + y * y)
    if length == 0 then 
        lemon.physics.set_velocity(ent, 0, 0)
        if currentDirection ~= IDLE then
            currentDirection = IDLE
            ent.animator:start("idle")
        end
        return
    end

    x, y = x / length, y / length

    lemon.physics.move_position(ent, lemon.vec2(position.x + x * speed * delta, position.y + y * speed * delta), delta)
    animation_frame(ent, x, y)
end
