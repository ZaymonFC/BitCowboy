const getPixels = require('get-pixels')

const numberOfTiles = 7
const tileSize = 16
const tilesPerRow = 6

const fs = require('fs')

getPixels('Tileset.png', (err, pixels) => {
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
            const pixel = pixels.get(col, row, 0)
            if (pixel === 255) {
                data.push(0)
            } else {
                data.push(1)
            }
        }
    }
    console.log(data)
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
    console.log(arrayString)

    fs.writeFileSync('generatedArray.txt', arrayString)
}