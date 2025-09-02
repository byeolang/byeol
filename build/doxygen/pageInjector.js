window.addEventListener('load', function() {
    // inject pages into upper navigation bar:
    let pages = NAVTREE[0][2];
    let dest = menudata.children

    for(const page of pages) {
        let name = page[0];
        let href = page[1];

        dest.push({
            text: page[0],
            url: page[1]
        });
    }
});
