/* entryAdd.js
 *  - adds new entry to database based off user entered form
 *  - adds new entry to end of visible table
 */
document.getElementById('entry-add').addEventListener('submit', (e) => {
    e.preventDefault();
    let req = new XMLHttpRequest();
    let data = {};
    let inputs = document.getElementsByClassName('entry-add-input');
    for (let i of inputs) {
        data[i.name] = i.checked
            ? 1
            : i.value;
    }
    //  add entry via ajax
    req.open('POST', '/add', true);
    req.setRequestHeader('Content-Type', 'application/json');
    //  update dom
    req.addEventListener('load', () => {
        if (req.status >= 200 && req.status < 400) {
            let _req = new XMLHttpRequest();
            let _id = JSON.parse(req.response).insertId;
            _req.open('POST', '/byId', true);
            _req.setRequestHeader('Content-Type', 'application/json');
            _req.addEventListener('load', () => {
                if (_req.status >= 200 && _req.status < 400) {
                    let table = document.getElementById('entry-table');
                    let row = table.insertRow(-1);
                    let _data = JSON.parse(_req.response)[0];
                    for (let i in _data) {
                        if (i !== 'id') {
                            let td = document.createElement('td');
                            let text = document.createTextNode(_data[i]);
                            td.appendChild(text);
                            row.appendChild(td);
                        }
                    }
                    let td = document.createElement('td');
                    td.dataset.id = JSON.parse(req.response).insertId;
                    let editBtn = document.createElement('input');
                    let deleteBtn = document.createElement('input');
                    editBtn.type = deleteBtn.type = 'button';
                    editBtn.value = 'Edit';
                    deleteBtn.value = 'Delete';
                    editOnClick(editBtn);
                    deleteOnClick(deleteBtn);
                    td.appendChild(editBtn);
                    td.appendChild(deleteBtn);
                    row.appendChild(td);
                }
            });
            _req.send(JSON.stringify({id: _id}));
        }
    });
    req.send(JSON.stringify(data));
});
