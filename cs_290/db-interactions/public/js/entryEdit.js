/* entryEdit.js
 *  - edits selected entry in databse
 *  - edits selected entry in table
 */
// form for single entry
const edit_form = document.getElementById('entry-edit-form');
// save edit
edit_form.addEventListener('submit', (e) => {
    e.preventDefault();
    let data = {};
    let inputs = document.getElementsByClassName('entry-edit-input');
    for (let i of inputs) {
        data[i.name] = i.value;
    }
    let req = new XMLHttpRequest();
    req.open('POST', '/edit', true);
    req.setRequestHeader('Content-Type', 'application/json');
    req.addEventListener('load', () => {
        if (req.status >= 200 && req.status < 400) {
            let _req = new XMLHttpRequest();
            _req.open('POST', '/byId', true);
            _req.setRequestHeader('Content-Type', 'application/json');
            _req.addEventListener('load', () => {
                if (_req.status >= 200 && _req.status < 400) {
                    let _data = JSON.parse(_req.response)[0];
                    let rows = document.getElementById('entry-table-data').children;
                    for (let row of rows) {
                        if (+row.lastElementChild.dataset.id === +_data['id']) {
                            row.children[0].innerHTML = _data['name'];
                            row.children[1].innerHTML = _data['reps'];
                            row.children[2].innerHTML = _data['weight'];
                            row.children[3].innerHTML = _data['date'];
                            row.children[4].innerHTML = _data['lbs'];
                        }
                    }
                }
            });
            _req.send(JSON.stringify({id: data['id']}));
        }
    });
    req.send(JSON.stringify(data));
});

// entries edit button click handler
const editOnClick = (item) => {
    item.addEventListener('click', (e) => {
        e.preventDefault();
        // get rows data
        let req = new XMLHttpRequest();
        req.open('POST', '/byId', true);
        req.setRequestHeader('Content-Type', 'application/json');
        req.addEventListener('load', () => {
            if (req.status >= 200 && req.status < 400) {
                let data = JSON.parse(req.response)[0];
                for (let i in data) {
                    for (let j of edit_form.children) {
                        if (i === j.name) {
                            j.value = data[i];
                            j.checked = data[i];
                        }
                    }
                }
                // show edit form
                document.getElementById('entry-edit-form').classList.toggle('hidden');
            }
        });
        req.send(JSON.stringify({id: item.parentNode.dataset.id}));
    });
};
// inital load
const edits = document.getElementsByClassName('entry-edit');
for (let edit of edits) {
    editOnClick(edit);
}
