>>>>>>>>>>+
[
    -
    >>>>
    ,
    ----------
    [
        ++++++++++
        >>
        ++++++++++              x1   0 10 0 0
        [-<++++++++++>]         x1 100  0 0 0
        <<
        [
            >                   x1   A  t1  t2 t3
            [->+>+<<]           x1   0  A   A  0
            >>
            [-<<+>>]            x1 A  A     0  0
            +<                  x1 A  A     1  0
            [                   if (A)
                >-<                 //      0
                <->               dec(A)
                [-]             x1 A' 0     0  0
            ]
            >
            [                   if (t2)
                >+<             inc(t3)
                [-]
            ]
            <<<
            -
        ]
        >>+                     x1  x2  t1  t2  t3 // set t1 = 1
        <
        [                       if (x2) // 0 x2 0 0 0
            >
            +++++++++
            [-<<++++++++++>>]  100 x2 0 0 0
            <
            [-<->]             set x1 = 100 `minus` x2
        ]
        >>>
        [                      if (t3)
            <<-                t1 = 0
            <
            ++++++++++         x2 = 10
            [-<++++++++++>]    x1 = 100
            >>>
            [-<<<<+>>>>]         set x1 = x1 `plus` t3
        ]

        <<
        [                           we have d
            =======copy only the first char as a marker=============
            -
            <<<[<]<<                go to beginning input
            +                       put a marker at the beginning
            >>>                     1__ab***z_***
                                    ___^___________
            [                       copy first char 2 times to the end
                [>]>
                +>+
                <<<[<]<<[>]>>       1__Ab***z_aa_
                -
                                    ___^__________
            ]
            <<<->>>                 delete marker
            >[>]>>                  go to the end
            [                       copy a back to the beginning
                <<<[<]<<            leave 2 spaces
                +
                >>>[>]>>            get back
                -
            ]
            ========copy the rest=====================
            <<<[<]>                 go to beginning
                                    a__bc***z_a_
                                    ___^____________
            [
                -                   decrement j***
                [>]>[>]
                +>+                 ***and put markers at the end
                [<]<[<]>
                                    a**i__Jk***z_a**i11_*** where J = j minus 1
                                    ______^______________

                [                   copy first char 2 times to the end
                    [>]>[>]
                    <+<+
                    [<]<[<]<<[>]>>
                    -
                ]
                >[>]>[>]            go to the end
                >+<<                put a marker
                                    a***iJ__k***z_a**ijj_1***
                                    ___________________^_____
                [                   copy j back to the beginning
                    <[<]<[<]<<
                    +
                    >>>[>]>[>]>[<]<
                    -
                ]
                >>-<<               delete the marker
                <[<]<[<]
                >                   get next char
            ]
            =============concatenate the 2 strings==========
            ab***z__ab***z***
            _______^__________
            >
            [
                [
                    -
                    <<<
                    +
                    >>>
                ]
                >
            ]
            <<
        ]
        <
        ,
        ----------
    ]
    <
    [
        >>>+<<<
        [<]
        >
        [.[-]>]
    ]
    ++++++++++.----------

    >>
]
