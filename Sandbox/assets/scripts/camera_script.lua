camera_script = {}

local player = {}

function camera_script.on_start (ent)
    player = ent:get_entity("Player")
end

function camera_script.on_physics (ent, delta)
    player_position = lemon.get_transform(player).position
    camera_position = lemon.get_transform(ent).position
    lemon.transform_system.move_to(ent, lemon.transform_system.lerp(camera_position, player_position, 3 * delta))
end
