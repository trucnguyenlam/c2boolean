



enum blocktype {thread0, thread1, thread0noloc, thread1noloc, want, have};


/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/******                                                                                    *******/
/******                               Reachability Algorithm                               *******/
/******                                                                                    *******/
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/




// module init is assigned s_mod=2

mu bool Init_Reach(
 PrCount s_pc,
 Local   s_CL,
 Global  s_G
)
 s_pc    <  s_CL,
 s_CL    <  s_G
(
  false

  // initial conf

  | initPC(s_pc)


  // forward propagation on internal transitions 

  |  ( exists 
           PrCount t_pc,
           Local   t_CL,
           Global  t_G.
           (   Init_Reach( t_pc, t_CL, t_G )
               &(t_G.v1=0 | ( t_G.v1=1 & t_CL.v0=1 ) )
               &( ( programInt1( 2, t_pc, s_pc, t_CL, s_CL, t_G, s_G )
                    & CopyVariables_ProgramInt( 2, t_pc, t_CL, s_CL, t_G, s_G )
                  )
                  | programInt3( 2, t_pc, s_pc, t_CL, s_CL, t_G, s_G )
                )
           )
     )

  | ( exists PrCount t_pc.
           (     Init_Reach( t_pc, s_CL, s_G )
               &(s_G.v1=0 | ( s_G.v1=1 & s_CL.v0=1 ) )
               & programInt2( 2, t_pc, s_pc, s_CL, s_G )
           )
      )

);

#size Init_Reach;





bool GlobalInit(Global CG)
( exists 
         Module  s_mod,
         PrCount s_pc,
         Local   s_CL.
              (   Init_Reach( s_pc, s_CL, CG )
                & Target_Init(s_pc )
              )
);


/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/



bool copy_g_and_h_3( Globals s_G, Globals t_G) 
 s_G ~+ t_G
(   true
    & s_G.g0 =t_G.g0 & s_G.h0 =t_G.h0 
    & s_G.g1 =t_G.g1 & s_G.h1 =t_G.h1 
    & s_G.g2 =t_G.g2 & s_G.h2 =t_G.h2 
    & s_G.g3 =t_G.g3 
);

bool copy_g_and_h_2( Globals s_G, Globals t_G) 
 s_G ~+ t_G
(   true
    & s_G.g0 =t_G.g0 & s_G.h0 =t_G.h0 
    & s_G.g1 =t_G.g1 & s_G.h1 =t_G.h1 
    & s_G.g2 =t_G.g2 
    & s_G.g3 =t_G.g3 & s_G.h3 =t_G.h3 
);


bool copy_g_and_h_1( Globals s_G, Globals t_G) 
 s_G ~+ t_G
(   true
    & s_G.g0 =t_G.g0 & s_G.h0 =t_G.h0 
    & s_G.g1 =t_G.g1 
    & s_G.g2 =t_G.g2 & s_G.h2 =t_G.h2 
    & s_G.g3 =t_G.g3 & s_G.h3 =t_G.h3 
);


bool copy_g_and_h_0( Globals s_G, Globals t_G) 
 s_G ~+ t_G
(   true
    & s_G.g0 =t_G.g0 
    & s_G.g1 =t_G.g1 & s_G.h1 =t_G.h1 
    & s_G.g2 =t_G.g2 & s_G.h2 =t_G.h2 
    & s_G.g3 =t_G.g3 & s_G.h3 =t_G.h3 
);




/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/******                                                                                    *******/
/******                               Reachability Algorithm                               *******/
/******                                                                                    *******/
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/





mu bool Sequ_Reach(
 blocktype  s_block,
 CS         s_r,
 Module     s_mod,
 PrCount    s_pc,
 Local      s_CL,
 Globals    s_G
)
 s_r     <  s_block,
 s_block <  s_mod,
 s_mod   <  s_pc,
 s_pc    <  s_CL,
 s_CL    <  s_G
( false

  // early termination


  | ( exists 
            blocktype  t_block,
            CS         t_r,
            Module     t_mod,
            PrCount    t_pc,
            Local      t_CL,
            Globals    t_G.
        (   Sequ_Reach( t_block, t_r, t_mod, t_pc, t_CL, t_G )
          & (   target( t_mod, t_pc )
              & ( t_block=thread0 | t_block=thread1 )
            )
        )
     )




  /*********************************************************************************/
  // thread1 (first block)


  // initial conf
  |  (
         s_block = thread1
         & initPC(s_pc) 
         & s_mod=1
         & GlobalInit(s_G.g0)
         & s_r=0
         & s_G.h0=s_G.g0
         & s_G.h1=s_G.g1
         & s_G.h2=s_G.g2
         & s_G.h3=s_G.g3  //PARAM
         &  ( forall PrCount t_pc. (!target( 2, t_pc)))
     )

   


  // increase round_nuber for thread1 (the first block)

  | (    s_block=thread1
//         & ( s_r != 0 )
         & ( exists CS t_r.
              (   true
                  & increaseCS( t_r, s_r )
                  & Sequ_Reach( thread1, t_r, s_mod, s_pc, s_CL, s_G ) //T1
                  & ( exists Globals a_G.
                       (     Sequ_Reach( have, t_r, s_mod, s_pc, s_CL, a_G ) // HAVE    
                           & copy_g_h( a_G, s_G, t_r )
                           & folding( s_G, a_G, s_r )
                       )
                    ) 
              )
          )
    )

/*
                                    |--------------|  
                                    | T1   ||  H   | 
                                    |      ||      | 
                                    | s_G  || a_G  | 
                                    |      ||      | 
                                    |      ||      | 
                                    |      ||      |
                                    |--------------|
*/

  /*********************************************************************************/

   // forgetting local states for thread1 or thread0  
  | ( exists blocktype  t_block,
             Module     t_mod,
             PrCount    t_pc,
             Local      t_CL,
             Globals    t_G.
             (    Sequ_Reach( t_block, s_r, t_mod, t_pc, t_CL, t_G )
                & ( false
                    |  ( (s_block=thread1noloc) & (t_block=thread1) )
                    |  ( (s_block=thread0noloc) & (t_block=thread0) )  
                  )
                & copy_g_g( t_G, s_G, s_r )
                & copy_h_h( t_G, s_G, s_r )
             )
    )


  /*********************************************************************************/
  // thread0 (blocks that are iterated)

  // initial conf

  |  (   s_block = thread0
         & s_r=0
         & initPC(s_pc)
         & s_mod=0  
         & ( exists  Globals t_G.
                  (  (    Sequ_Reach( thread1noloc, s_r, s_mod, s_pc, s_CL, t_G )
                        | Sequ_Reach( thread0noloc, s_r, s_mod, s_pc, s_CL, t_G )
                     )
                     & t_G.h0 = s_G.g0
                  )
           )
         & s_G.h0=s_G.g0
         & s_G.h1=s_G.g1
         & s_G.h2=s_G.g2
         & s_G.h3=s_G.g3
     )


  // increase round_number for thread0
  | (    s_block=thread0
//&s_r=1
         & ( s_r!=0 ) // s_r>0
         & ( exists CS t_r.
               (     Sequ_Reach( thread0, t_r, s_mod, s_pc, s_CL, s_G ) //T0
                   & increaseCS( t_r, s_r )
               )
           )
         & ( exists Globals b_G.
              (   ( exists CS t_r, Globals a_G.
                      (   Sequ_Reach( have, t_r, s_mod, s_pc, s_CL, a_G ) //HAVE, s_mod, s_pc, s_CL are free
                        & (   copy_g_h( a_G, s_G, t_r )
                            & copy_h_h( a_G, b_G, t_r )
                            & increaseCS( t_r, s_r )
                          )
                      )
                  )           
                & Sequ_Reach( want, s_r, s_mod, s_pc, s_CL, b_G ) //WANT s_mod, s_pc, s_CL are free
                & copy_g_g( b_G, s_G, s_r )
 
              )
           )
    )
                              
/*
                                     ------------------  
                                    |  ------  ------  |
                                    | | T0   ||  H   | |
                                    | |      ||      | |
                                    | | s_G  || a_G  | |
                                    | |      ||      | |
                                    |   ------ ------  |
                                    |        b_G    W  |
                                     -----------------
*/

  /*********************************************************************************/
  // want block


  // want blocks generated by thread1 (the first block)

  | (    s_block=want
         & ( s_r!=0 )
         & ( exists CS t_r.
              (   Sequ_Reach( have, t_r, s_mod, s_pc, s_CL, s_G ) //HAVE
                & increaseCS( t_r, s_r )
              )
           )
         & ( exists Globals a_G.
              (   Sequ_Reach( thread1noloc, s_r, s_mod, s_pc, s_CL, a_G ) //T1
                & (    copy_g_h( s_G, a_G, s_r )
                     & folding( a_G, s_G, s_r )
                  )
              )
           )
    )

/*
                                       ------  ------  
                                      | T1   ||  H   | 
                                      |      ||      | 
                                      | a_G  || s_G  | 
                                      |      ||      | 
                                      |      | ------  
                                      |      |  
                                       ------

*/
  // want blocks generated by thread0

  | (    s_block=want
         & ( s_r!=0 )
         & ( exists CS t_r.
              (   Sequ_Reach( have, t_r, s_mod, s_pc, s_CL, s_G ) //HAVE
                & increaseCS( t_r, s_r )
              )
           )
         & ( exists Globals b_G.
              (   (exists Globals a_G.
                      (   Sequ_Reach( thread0noloc, s_r, s_mod, s_pc, s_CL, a_G ) //T0
                        & (    copy_g_h( s_G, a_G, s_r )
                             & copy_g_g( a_G, b_G, s_r )
                          )
                      )
                  )
                & Sequ_Reach( want, s_r, s_mod, s_pc, s_CL, b_G )
                & copy_h_h( b_G, s_G, s_r )
              )
           )
    )

/*
                                   |----------------|  
                                   | | T0   ||  H   | 
                                   | |      ||      | 
                                   | | a_G  || s_G  | 
                                   | |      ||      | 
                                   | |      | ------| 
                                   | |      | b_G  W|
                                   |----------------|
*/
  /*********************************************************************************/
  // have block 


  // have blocks generated from the last block
  | (    s_block=have
         & copy_g_h( s_G, s_G, s_r )
         & (
                (   !(s_r=0) 
                  & !(s_r=3) //3 is a PARAM for 4 rounds
                  &  Sequ_Reach( want, s_r, s_mod, s_pc, s_CL, s_G ) 
                )
              | (    s_r=0
                  &  Sequ_Reach( thread0noloc, s_r, s_mod, s_pc, s_CL, s_G )

                )
           )
    )

  // have blocks generated by thread0 (not the last block)
  | (    s_block=have
         & ( s_r=0 | Sequ_Reach( want, s_r, s_mod, s_pc, s_CL, s_G ) )
         & ( exists Globals b_G.
                  (     ( exists Globals a_G.   
                            (    Sequ_Reach( thread0noloc, s_r, s_mod, s_pc, s_CL, a_G )
                               & (   copy_g_g( s_G, a_G, s_r )
                                   & copy_g_h( b_G, a_G, s_r )
                                 )
                            )
                        )
                     &  (    Sequ_Reach( have, s_r, s_mod, s_pc, s_CL, b_G )
                           & copy_h_h( s_G, b_G, s_r )
                        )
                  )
           )

    )

/*
                                    |--------------|  
                                    | T0   ||  H   | 
                                    |      ||      | 
                                    | a_G  || b_G  | 
                                    |      ||      | 
                                    |      ||      | 
                                    |      ||      |
                                    |--------------|
                                          s_G    W
*/

 
//*********************************************************************************/
// forward propagation on internal transitions 


//*************** 0
  |  ( 
        ( s_r=0 & s_G.g0.v1 =0 & (s_block=thread0 | s_block=thread1) )
      & (exists 
           PrCount t_pc,
           Local   t_CL,
           Globals t_G.
           (    (   Sequ_Reach( s_block, s_r, s_mod, t_pc, t_CL, t_G )
                  & copy_g_and_h_0(s_G,t_G)
                )
               &(t_G.h0.v1=0 | ( t_G.h0.v1=1 & t_CL.v0=1 ) )
               &(
                  ( programInt1(s_mod,t_pc,s_pc,t_CL,s_CL,t_G.h0,s_G.h0)
                    & CopyVariables_ProgramInt(s_mod,t_pc,t_CL,s_CL,t_G.h0,s_G.h0)
                  )
                  | programInt3( s_mod, t_pc, s_pc, t_CL, s_CL, t_G.h0, s_G.h0 )
                )
           )
      )
    )

  | (    (s_G.g0.v1 =0 & s_r=0 & (s_block=thread0 | s_block=thread1) )
       & (s_G.h0.v1=0 | ( s_G.h0.v1=1 & s_CL.v0=1 ) )
       & (exists PrCount t_pc.
             (     Sequ_Reach( s_block, s_r, s_mod, t_pc, s_CL, s_G )
                 & programInt2(s_mod,t_pc,s_pc,s_CL,s_G.h0)
             )
         )
    )

                 

//*************** 1
  |  ( 
        (s_r=1  &  s_G.g1.v1=0 & (s_block=thread0 | s_block=thread1) )
      & (exists 
           PrCount t_pc,
           Local   t_CL,
           Globals t_G.
           (    ( Sequ_Reach( s_block, s_r, s_mod, t_pc, t_CL, t_G )
                  & copy_g_and_h_1(s_G,t_G)
                )
               &(t_G.h1.v1=0 | ( t_G.h1.v1=1 & t_CL.v0=1 ) )
               &(
                  ( programInt1(s_mod,t_pc,s_pc,t_CL,s_CL,t_G.h1,s_G.h1)
                    & CopyVariables_ProgramInt(s_mod,t_pc,t_CL,s_CL,t_G.h1,s_G.h1)
                  )
                  | programInt3( s_mod, t_pc, s_pc, t_CL, s_CL, t_G.h1, s_G.h1 )
                )
           )
      )
    )


  | (
       ( s_G.g1.v1=0 & s_r=1 & (s_block=thread0 | s_block=thread1) )
     & (s_G.h1.v1=0 | ( s_G.h1.v1=1 & s_CL.v0=1 ) )
     & (exists PrCount t_pc.
           (     Sequ_Reach( s_block, s_r, s_mod, t_pc, s_CL, s_G )
               & programInt2(s_mod,t_pc,s_pc,s_CL,s_G.h1)
           )
       )
    )




//*************** 2
  |  ( 
        ( s_r=2  &  s_G.g2.v1=0 & (s_block=thread0 | s_block=thread1) )
      & (exists
           PrCount t_pc,
           Local   t_CL,
           Globals t_G.
           (    ( Sequ_Reach( s_block, s_r, s_mod, t_pc, t_CL, t_G )
                  & copy_g_and_h_2(s_G,t_G)
                )
               &(t_G.h2.v1=0 | ( t_G.h2.v1=1 & t_CL.v0=1 ) )
               &(
                  ( programInt1(s_mod,t_pc,s_pc,t_CL,s_CL,t_G.h2,s_G.h2)
                    & CopyVariables_ProgramInt(s_mod,t_pc,t_CL,s_CL,t_G.h2,s_G.h2)
                  )
                  | programInt3( s_mod, t_pc, s_pc, t_CL, s_CL, t_G.h2, s_G.h2 )
                )
           )
      )
    )


  | (
       ( s_G.g2.v1=0 & s_r=2 & (s_block=thread0 | s_block=thread1) )
     & (s_G.h2.v1=0 | ( s_G.h2.v1=1 & s_CL.v0=1 ) )
     & (exists PrCount t_pc.
           (     Sequ_Reach( s_block, s_r, s_mod, t_pc, s_CL, s_G )
               & programInt2(s_mod,t_pc,s_pc,s_CL,s_G.h2)
           )
       )
    )




//*************** 3

  |  (  (s_r=3  &  s_G.g3.v1=0 & (s_block=thread0 | s_block=thread1) )
      & (exists 
           PrCount t_pc,
           Local   t_CL,
           Globals t_G.
           (    (   Sequ_Reach( s_block, s_r, s_mod, t_pc, t_CL, t_G )
                  & copy_g_and_h_3(s_G,t_G)
                )
               &(t_G.h3.v1=0 | ( t_G.h3.v1=1 & t_CL.v0=1 ) )
               &(
                  ( programInt1(s_mod,t_pc,s_pc,t_CL,s_CL,t_G.h3,s_G.h3)
                    & CopyVariables_ProgramInt(s_mod,t_pc,t_CL,s_CL,t_G.h3,s_G.h3)
                  )
                  | programInt3( s_mod, t_pc, s_pc, t_CL, s_CL, t_G.h3, s_G.h3 )
                )
           )
      )
    )

  | (  (s_G.g3.v1=0 & s_r=3 & (s_block=thread0 | s_block=thread1) )
     & (s_G.h3.v1=0 | ( s_G.h3.v1=1 & s_CL.v0=1 ) )
     & (exists PrCount t_pc.
           (     Sequ_Reach( s_block, s_r, s_mod, t_pc, s_CL, s_G )
               & programInt2(s_mod,t_pc,s_pc,s_CL,s_G.h3)
           )
       )
    )


);

#size Sequ_Reach;


/******************************************************************************************************/
//                                       Reachabibility check                                         *
/******************************************************************************************************/


(

    ( exists 
            PrCount t_pc,
            Local   t_CL,
            Global  t_G.
        (   Init_Reach( t_pc, t_CL, t_G )
          & target( 2, t_pc)
        )
     )

  | ( exists 
            blocktype  t_block,
            CS         t_r,
            Module     t_mod,
            PrCount    t_pc,
            Local      t_CL,
            Globals    t_G.
        (   Sequ_Reach( t_block, t_r, t_mod, t_pc, t_CL, t_G )
          & (   target( t_mod, t_pc )
              & ( t_block=thread0 | t_block=thread1 )
            )
        )
     )


);



