const getPixels = require('get-pixels')

const numberOfTiles = 12
const tileSize = 16
const tilesPerRow = 6

const fs = require('fs')
const inputFile = 'Tileset.png'

getPixels(inputFile, (err, pixels) => {
    if (err) throw err
    console.log('Got Pixels:', pixels.shape.slice())

    const tileData = createTileArrays(pixels, numberOfTiles, tileSize)
    createCArrays(tileData)
})

function createTileArrays(pixels, numberOfTiles, tileSize) {
    let tileData = []

    for (let i = 0; i < numberOfTiles; i++) {
        tileData.push(createTileData(pixels, i, tileSize))
    }

    return tileData
}

function createTileData(pixels, tileNumber, tileSize) {
    // Calculate the coords of the current tile
    const coord = {
        x: (tileNumber * tileSize) % (tileSize * tilesPerRow),
        y: (Math.floor(tileNumber / tilesPerRow)) * tileSize
    }


    let data = []
    for (let row = coord.y; row < coord.y + tileSize; row++) {
        // Write the row into the data array
        for (let col = coord.x; col < coord.x + tileSize; col++) {
            const pixel = pixels.get(col, row, 1)
            if (pixel > 10) {
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
    for (let tileNumber = 0; tileNumber < numberOfTiles; tileNumber++) {
        let string = '{ '
        for (let pixel of tileData[tileNumber]) {
            string += `${pixel},`
        }
        string += ' }'
        cArrayStrings.push(string)
    }

    let arrayString = '{\n'
    for (let string of cArrayStrings) {
        arrayString += ` ${string}, \n`
    }
    arrayString += '\n}'

    const filename = 'generatedArray.txt'
    fs.writeFileSync(filename, arrayString)
    console.log('Created C Array from TileSet')
    console.log(`Input: ${inputFile}\nCreated ${numberOfTiles} tiles.\nOutput: ${filename}`)
}