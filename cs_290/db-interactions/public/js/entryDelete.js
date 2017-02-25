/* entryDelete.js
 *  - deletes selected entry from visible table
 *  - deletes selected entry from database
 */
const deleteOnClick = (item) => {
    item.addEventListener('click', function(e) {
        e.preventDefault();
        let req = new XMLHttpRequest();
        let data = {id:this.parentNode.dataset.id};
        // update dom
        document.getElementById('entry-table').deleteRow(this.parentNode.parentNode.rowIndex);
        // delete entry
        req.open('POST', '/delete', true);
        req.setRequestHeader('Content-Type', 'application/json');
        req.send(JSON.stringify(data));
    });
};
//  inital load
const deletes = document.getElementsByClassName('entry-delete');
for (let del of deletes) {
    deleteOnClick(del);
}
