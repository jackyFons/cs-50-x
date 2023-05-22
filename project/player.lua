local SCREEN_SIZE = 640
local rocket = nil

Player = {}

function Player:load()
	-- Retrieve image
	rocket = love.graphics.newImage("Assets/temp_rocket1.png")

	-- Initialize starting position
	self.x = SCREEN_SIZE / 2
	self.y = SCREEN_SIZE - 100
	self.h = rocket:getHeight()
	self.w = rocket:getWidth()
	self.r = 0
	self.ox = 1
	self.oy = 1
	self.speed = 350

	-- Add physics to player and give it a body for collisions 
	self.rocket = {}
	self.rocket.body = love.physics.newBody(world, self.x, self.y, "dynamic")
	self.rocket.shape = love.physics.newRectangleShape(self.w, self.h)
	self.rocket.fixture = love.physics.newFixture(self.rocket.body, self.rocket.shape)
end


function Player:draw()
	love.graphics.draw(rocket, self.x, self.y, self.r, 1, 1, self.ox, self.oy)
end


function Player:update(dt)
	self:move(dt)
	self:bounds()
end


function Player:move(dt)
	-- Moves player based on keyboard input
	if love.keyboard.isDown("up") then
		print(self.x, self.y)
		self.r = 0
		self.ox = 1
		self.oy = 1
		self.y = self.y - (dt * self.speed)		
	elseif love.keyboard.isDown("down") then
		print(self.x, self.y)
		self.r = math.pi
		self.ox = self.w
	    self.oy = self.h
	    self.y = self.y + (dt * self.speed)
	elseif love.keyboard.isDown("left") then
	    self.r = (3 * math.pi) / 2
	    self.ox = 5 * self.w / 4
	    self.oy = (self.w / 4)
	    self.x = self.x - (dt * self.speed)
	elseif love.keyboard.isDown("right") then
		self.r = math.pi / 2
	    self.ox = -self.w / 4
	    self.oy = 5 * self.w / 4
	    self.x = self.x + (dt * self.speed)
	end
end


function Player:bounds()
	-- Makes sure player is in bounds
	if self.y < 0 then
		self.y = 0
	elseif (self.y + self.h) > SCREEN_SIZE then
		self.y = SCREEN_SIZE - self.h
	elseif self.x < (self.w / 4) then 
		self.x = self.w / 4
	elseif self.x > (SCREEN_SIZE - (11 * self.w / 8)) then
		self.x = (SCREEN_SIZE - (11 * self.w / 8))
	end
end



--if love.keyboard.isDown("up") and love.keyboard.isDown("left") then
	--	self.r = (7 * math.pi) / 4
	--	self.ox = (self.h / 2)
	--	self.oy = 1
	--elseif love.keyboard.isDown("up") and love.keyboard.isDown("right") then
	--	self.r = math.pi / 4
	--	self.ox = (self.h)
	--	self.oy = 1