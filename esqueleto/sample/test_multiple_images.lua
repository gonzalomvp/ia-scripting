function createImage(texture_name, char_size)
	gfxQuad = MOAIGfxQuad2D.new()
	gfxQuad:setTexture(texture_name)
	char_size = 64
	gfxQuad:setRect(-char_size/2, -char_size/2, char_size/2, char_size/2)
	gfxQuad:setUVRect(0, 0, 1, 1)
	return gfxQuad
end

MOAISim.openWindow("game", 1024, 768)

viewport = MOAIViewport.new()
viewport:setSize (1024, 768)
viewport:setScale (1024, -768)

layer = MOAILayer2D.new()
layer:setViewport(viewport)
MOAISim.pushRenderPass(layer)

char_size = 64
gfxQuad = createImage("dragon.png", char_size)
gfxQuad1 = createImage("dragonDead.png", char_size)
gfxQuad2 = createImage("dragonAttack.png", char_size)

prop = MOAIProp2D.new()
prop:setDeck(gfxQuad)
entity  = Character.new()

-- Add prop to be the renderable for this character
entity:addImage(gfxQuad)    -- SetImage(0)  dragon.png
entity:addImage(gfxQuad1)   -- SetImage(1)  dragonDead.png
entity:addImage(gfxQuad2)   -- SetImage(2)  dragonAttack.png

entity:setProp(prop, layer)

-- Start the caracter (allow calls to OnUpdate)
entity:start()

entity:setLoc(-200, -200)
entity:setRot(0)
entity:setLinearVel(20, 20)
entity:setAngularVel(30)

-- Enable Debug Draw
debug = MOAIDrawDebug.get();
layer:setDrawDebug(debug)
-- Add this character to draw debug
MOAIDrawDebug.insertEntity(entity)
mouseX = 0
mouseY = 0

function onClick(down)
  --entity:setLoc(mouseX, mouseY)
 -- entity:setRot(0)
end

function pointerCallback(x, y)
    mouseX, mouseY = layer:wndToWorld(x, y)
end

MOAIInputMgr.device.mouseLeft:setCallback(onClick)
MOAIInputMgr.device.pointer:setCallback(pointerCallback)