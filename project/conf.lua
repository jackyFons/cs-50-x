function love.conf(t)
	t.window.title = "Game" -- Window name
	t.window.version = "1.0"
	t.window.icon = nil -- Window icon
	t.window.height = 640 -- Window height
	t.window.width = 640 -- Window width

    t.modules.event = true             
    t.modules.joystick = false
    t.modules.physics = true
    t.modules.system = true
    t.modules.thread = true
    t.modules.timer = true
    t.modules.touch = false
    t.modules.video = false

    t.console = true
end