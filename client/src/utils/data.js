export function mapToColumns(obj, include = [], exclude = []) {
    const keys = include.length ?
        include :
        Object.keys(obj);

    return keys
        .filter((key) => !exclude.includes(key))
        .map((key) => ({
            key: obj[key].name,
            name: obj[key].label,
            fieldName: obj[key].name,
            ariaLabel: obj[key].ariaLabel,
            minWidth: obj[key].minWidth || 100,
            maxWidth: obj[key].maxWidth || 200,
            data: obj[key].data || {},
            notSortable: obj[key].notSortable || false,
            isSortedDescending: obj[key].isSortedDescending || undefined,
        }));
}