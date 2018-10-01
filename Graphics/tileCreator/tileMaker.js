const getPixels = require('get-pixels')

let settings = {
    numberOfTiles: null,
    tileHeight: null,
    tileWidth: null,
    columns: null
}

const presets = {
    tileSet: {
        inputFile: '../Tileset.png',
        outputFile: '../Tileset.c',
        varialeName: 'tiles',
        numberOfTiles: 12,
        tileWidth: 16,
        tileHeight: 16,
        columns: 6,
    },
    sprite: {
        inputFile: '../spriteSheet.png',
        outputFile: '../spriteSheet.c',
        varialeName: 'sprite',
        numberOfTiles: 3,
        tileWidth: 9,
        tileHeight: 12,
        columns: 3,
    }
}

const argv = require('yargs').argv

// Parse Commandline Arguments
if (argv.sprite) {
    settings = presets.sprite
} else {
    settings = presets.tileSet
}

const fs = require('fs')

getPixels(settings.inputFile, (err, pixels) => {
    if (err) throw err
    console.log('Got Pixels:', pixels.shape.slice())

    const tileData = createTileArrays(pixels, settings.numberOfTiles)
    createCArrays(tileData)
})

function createTileArrays(pixels, numberOfTiles) {
    let tileData = []

    for (let i = 0; i < numberOfTiles; i++) {
        tileData.push(createTileData(pixels, i))
    }

    return tileData
}

function createTileData(pixels, tileNumber) {
    // Calculate the coords of the current tile
    const coord = {
        x: (tileNumber * settings.tileWidth) % (settings.tileWidth * settings.columns),
        y: (Math.floor(tileNumber / settings.tileHeight)) * settings.tileHeight
    }

    let data = []
    for (let row = coord.y; row < coord.y + settings.tileHeight; row++) {
        // Write the row into the data array
        for (let col = coord.x; col < coord.x + settings.tileWidth; col++) {
            const pixel = pixels.get(col, row, 1)
            if (pixel === 0) {
                data.push(1)
            } else {
                data.push(0)
            }
        }
    }

    return data
}

function createCArrays(tileData) {
    let cArrayStrings = []
    for (let tileNumber = 0; tileNumber < settings.numberOfTiles; tileNumber++) {
        let pixels = ''
        for (let pixel of tileData[tileNumber]) {
            pixels += `${pixel},`
        }
        cArrayStrings.push(pixels)
    }

    let arrayString = `int ${settings.varialeName}[${settings.numberOfTiles}][${settings.tileHeight * settings.tileWidth}] = {\n`
    for (let singleArray of cArrayStrings) {
        arrayString += `    { ${singleArray.slice(0, -1)} }, \n`
    }
    arrayString += '};'

    fs.writeFileSync(settings.outputFile, arrayString)

    console.log('Created C Array from TileSet')
    console.log(`Input: ${settings.inputFile}\nCreated ${settings.numberOfTiles} tiles.\nOutput: ${settings.outputFile}`)
}