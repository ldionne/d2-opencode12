<!SLIDE subsection>
# Lock order inconsistencies


<!SLIDE centered-code>
# Example \#1

    @@@ cpp
    mutex A, B;
    thread t1([&] {
        scoped_lock a(A);
        scoped_lock b(B);
    });

    thread t2([&] {
        scoped_lock b(B);
        scoped_lock a(A);
    });


<!SLIDE centered-code>
# Example \#2

    @@@ cpp
    mutex A, B, C;
    thread t1([&] {
        scoped_lock a(A);
        scoped_lock b(B);
    });

    thread t2([&] {
        scoped_lock b(B);
        scoped_lock c(C);
    });

    thread t3([&] {
        scoped_lock c(C);
        scoped_lock a(A);
    });


<!SLIDE>
# Same principle but harder to catch


<!SLIDE smbullets>
# Why are they so vicious?

* Non deterministic
* Often uncaught by unit tests
* Difficult to reproduce


<!SLIDE smbullets>
# Possible solutions

* Never hold more than one lock at once
* Determine a hierarchy among locks and respect it
* Disturb thread scheduling to provoke hidden deadlocks
* Use an algorithm to break deadlocks when they happen


<!SLIDE>
## We would like to detect them automatically and before they happen
