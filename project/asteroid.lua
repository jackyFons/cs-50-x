Asteroid = {}
Asteroid.__index = Asteroid

AllAsteroids = {}

function Asteroid.new(x_loc, y_loc, x_speed, y_speed)
	local asteroid = setmetatable({}, Asteroid)

	-- Retrieve image
	asteroid.sprite = love.graphics.newImage("Assets/temp_asteroid1.png")

	-- Set values for in-game asteroids
	asteroid.rad = asteroid.sprite:getHeight() / 2
	asteroid.x = x_loc
	asteroid.y = y_loc
	asteroid.xs = x_speed
	asteroid.ys = y_speed

	asteroid.physics = {}
	asteroid.physics.body = love.physics.newBody(world, asteroid.x, asteroid.y, "dynamic")
	asteroid.physics.shape = love.physics.newCircleShape(asteroid.x, asteroid.y, asteroid.rad)
	asteroid.physics.fixture = love.physics.newFixture(asteroid.physics.body, asteroid.physics.shape)
	asteroid.physics.fixture:setSensor(true)

	table.insert(AllAsteroids, asteroid)

	return asteroid
end


function Asteroid:draw()
	for i, asteroid in ipairs(AllAsteroids) do
		love.graphics.draw(asteroid.sprite, asteroid.x, asteroid.y, 0, 1, 1, asteroid.rad, asteroid.rad)
	end
end


function Asteroid:update(dt)
	for i, asteroid in ipairs(AllAsteroids) do
		asteroid.x = asteroid.x + (asteroid.xs * dt)
		asteroid.y = asteroid.y + (asteroid.ys * dt)
	end
end