# Github Pages for Namu language

## How to update WASM file
* When you build wasm from the byeol repository, two files, `byeol.wasm` and `byeol.js`, appear.
* At this time, simply upload the `byeol.wasm` file as an asset to the release of the `byeol` repository.

### Why?
The `byeol.js` file created by this `emscripten` is written to basically search for wasm files in the local path and import them from an external source under certain conditions.

`byeol.js` in the `play` folder of this `gh-pages` branch has been modified to always import the wasm file from the outside as an absolute path.
