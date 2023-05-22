local background = nil

require("player")
require("asteroid")

function love.load()
    -- Load background image
    background = love.graphics.newImage("Assets/temp_background.jpg")

    -- Make world for collisions
    world = love.physics.newWorld(0, 0)
    
    Player:load()

    Asteroid.new(250, 250, 10, 10)
    Asteroid.new(350, 350, -10, 50) 
    print(tAst)
end


function love.draw()
    -- Draw background
    love.graphics.draw(background)
    love.graphics.push()
    Player:draw()
    Asteroid:draw()
    love.graphics.pop()
end


function love.update(dt)
    Player:update(dt)
    Asteroid:update(dt)
end