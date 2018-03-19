module.exports = {
    ROLE: {
        ID: 'id',
        NAME: 'name',
    },
    BADGE_TYPE: {
        ID: 'id',
        TYPE: 'type',
    },
    CAMPUS: {
        ID: 'id',
        NAME: 'name',
    },
    LOCATION: {
        ID: 'id',
        NAME: 'name',
    },
    CAMPUS_LOCATION: {
        ID: 'id',
        CAMPUS_ID: 'campusId',
        LOCATION_ID: 'locationId',
    },
    ADMIN: {
        ID: 'id',
        ROLE_ID: 'roleId',
        FIRST_NAME: 'firstName',
        LAST_NAME: 'lastName',
    },
    VISIT: {
        ID: 'id',
        DATE: 'date',
        CAMPUS_LOCATION_ID: 'campusLocationId',
    },
    VISITOR: {
        ID: 'id',
        VISIT_ID: 'visitId',
        BADGE_ID: 'badgeId',
        FIRST_NAME: 'firstName',
        LAST_NAME: 'lastName',
    },
};