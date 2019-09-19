// EXTENSION 2016-06-15

    // prendiamo una pagina si' e una no, in modo che lo spazio
    // comune non sia contiguo in memoria fisica
    vdf = reinterpret_cast<des_frame*>(vdf_start);

    frame_liberi = &vdf[0];

    natl i;

    for (i = 0; i < N_DF; i += 2)
    {
        vdf[i].livello = -1;
        vdf[i].prossimo_libero = &vdf[i + 2];
    }

    vdf[i - 2].livello = -1;

    vdf[i - 2].prossimo_libero = 0;

// EXTENSION 2016-06-15

