# accounts

    [ ] User accounts are supported (ie. there is data tied to specific users that only they can see or modify)
    [ ] the account system uses 3rd party provider (oauth...), there should be able to be an arbitrary number of accounts.
    [ ] Accounts *must* have access to some amount of account specific information that only they can either access or modify.

# data response format

    [ ] http://jsonapi.org/format/
    [ ] https://github.com/django-json-api/django-rest-framework-json-api/issues/241

# entity 1 - Animal

    [ ] has at least 4 properties - id, name, age, type, owner (just id?), nextVist, visits(just ids?), (self link), last updated, created
    [ ] create - id (auto), name, age, type, owner, nextVist ? (new visit + link it), visits (empty array), last updated + created (now)
        [ ] route
        [ ] model
        [ ] controller
        [ ] tests
        [ ] pdf
    [ ] read
        [ ] route
        [ ] model
        [ ] controller
        [ ] tests
        [ ] pdf
    [ ] update
        [ ] route
        [ ] model
        [ ] controller
        [ ] tests
        [ ] pdf
    [ ] delete
        [ ] route
        [ ] model
        [ ] controller
        [ ] tests
        [ ] pdf

# entity 2 - Visit

    [ ] has at least 4 properties - id, datetime start, datetime end, reason, vet, animal(just id?), (self link), last updated, created
    [ ] create
        [ ] route
        [ ] model
        [ ] controller
        [ ] tests
        [ ] pdf
    [ ] read
        [ ] route
        [ ] model
        [ ] controller
        [ ] tests
        [ ] pdf
    [ ] update
        [ ] route
        [ ] model
        [ ] controller
        [ ] tests
        [ ] pdf
    [ ] delete
        [ ] route
        [ ] model
        [ ] controller
        [ ] tests
        [ ] pdf

# deliverables

    [ ] postman test suite
    [ ] video of tests being ran
    [ ] pdf
    [ ] source code zip

[ ] There should be at least one relationship between entities
[ ] This (video) should demonstrate adding, viewing and removing things in a relationship
You will need to use some more advanced features of postman to do this because you will need to pull variable names and save them to variables
