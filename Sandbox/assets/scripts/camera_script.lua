camera_script = {}

local player = {}

function camera_script.on_start (ent)
    player = ent:get_entity("Player")
end

function camera_script.on_update (ent, delta)
    player_transform = lemon.get_transform(player)
    lemon.transform_system.move_to(ent, player_transform.position)
end
