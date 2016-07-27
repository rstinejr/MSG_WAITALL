# MSG_WAITALL

Explore the use of option MSG_WAITALL to simplify retrieving blocks of fixed
length using TCP.  

To build and test, clone the project, cd to directory MSG_WAITALL, and enter

    ```
    make clean run
    ```

## Why?

I was looking at the *man* page for recv and I noticed the description of option ```MSG_WAITFOR```:

    ```
    This flag requests that  the  operation  block  until  the  full
    request  is  satisfied.  However, the call may still return less
    data than requested if a signal is caught, an error  or  discon‐
    nect  occurs,  or the next data to be received is of a different
    type than that returned.
    ```

Whoa!

This must be to address a well known isue with TCP: it is a stream protocol that does not preserve record boundaries.
The chunks that are sent may differ in length and number from the chunks that are received. What TCP guarantees is that
after you put them together, the blob you recieve will be an exact replicat of the blob you sent.  

Over the decades, any time I've used TCP to implement a record-based protocol, I've cluttered the
code up with read-loops for pulling in an entire record, and now there is an option to have the OS
do that for you. So I thought I'd try to get it to work.

