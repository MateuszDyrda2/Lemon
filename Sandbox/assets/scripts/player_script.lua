player_script = { }

function player_script.test (ent, x, y, z) 
    lemon.logger.info(x .. y .. z)
    lemon.logger.warn(tostring(ent.handle))
end
