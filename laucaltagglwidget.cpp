#include "laucaltagglwidget.h"
#include "locale.h"

// DEFINE LOOK-UP TABLE
const unsigned char bridgeLutA[512] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff,
                                        0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00,
                                        0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00,
                                        0x00, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff,
                                        0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff,
                                        0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00,
                                        0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
                                        0x00, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                        0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                        0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00,
                                        0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff,
                                        0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
                                        0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00,
                                        0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00,
                                        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff,
                                        0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00,
                                        0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff,
                                        0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
                                        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
                                        0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
                                        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                        0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                        0xff, 0xff
                                      };

const unsigned char bridgeLutB[512] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                       0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff,
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
                                       0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
                                       0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00,
                                       0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00,
                                       0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
                                       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                       0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                       0xff, 0xff
                                      };

const unsigned char bridgeLutC[512] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
                                        0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                        0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
                                        0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                        0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
                                        0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
                                        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff,
                                        0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                        0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                        0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
                                        0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
                                        0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff,
                                        0xff, 0xff
                                      };

const unsigned char bridgeLutD[512] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
                                        0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                        0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
                                        0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                        0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
                                        0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
                                        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff,
                                        0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                        0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                        0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
                                        0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
                                        0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff,
                                        0xff, 0xff
                                      };

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUCalTagFilterWidget::LAUCalTagFilterWidget(LAUCalTagGLWidget *glwdgt, QWidget *parent) : QWidget(parent), glWidget(glwdgt)
{
    this->setLayout(new QVBoxLayout());
    this->setWindowTitle(QString("Curvature Dialog"));
    this->layout()->setContentsMargins(6, 6, 6, 6);
    this->layout()->setSpacing(10);

    QGroupBox *box = new QGroupBox(QString("Binarize Parameters"));
    box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    box->setLayout(new QGridLayout());
    box->layout()->setContentsMargins(6, 6, 6, 6);
    ((QGridLayout *)(box->layout()))->setColumnStretch(0, 100);
    ((QGridLayout *)(box->layout()))->setColumnMinimumWidth(2, 160);
    this->layout()->addWidget(box);

    iterSpinBox = new QSpinBox();
    iterSpinBox->setMinimum(1);
    iterSpinBox->setMaximum(5);
    iterSpinBox->setFixedWidth(80);
    iterSpinBox->setAlignment(Qt::AlignRight);
    connect(iterSpinBox, SIGNAL(valueChanged(int)), glWidget, SLOT(onSetIterations(int)));

    QLabel *label = new QLabel(QString("Gaussian Smoother Iterations:"));
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    label->setAlignment(Qt::AlignRight);
    ((QGridLayout *)(box->layout()))->addWidget(label, 0, 0, 1, 1, Qt::AlignRight);
    ((QGridLayout *)(box->layout()))->addWidget(iterSpinBox, 0, 1, 1, 1, Qt::AlignLeft);

    gausSpinBox = new QSpinBox();
    gausSpinBox->setMinimum(1);
    gausSpinBox->setMaximum(127);
    gausSpinBox->setFixedWidth(80);
    gausSpinBox->setAlignment(Qt::AlignRight);
    connect(gausSpinBox, SIGNAL(valueChanged(int)), glWidget, SLOT(onSetGaussianRadius(int)));

    label = new QLabel(QString("Guassian Smoother Radius:"));
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    label->setAlignment(Qt::AlignRight);
    ((QGridLayout *)(box->layout()))->addWidget(label, 0, 2, 1, 1, Qt::AlignRight);
    ((QGridLayout *)(box->layout()))->addWidget(gausSpinBox, 0, 3, 1, 1, Qt::AlignLeft);

    offsetSpinBox = new QDoubleSpinBox();
    offsetSpinBox->setMinimum(-1.0);
    offsetSpinBox->setMaximum(1.0);
    offsetSpinBox->setSingleStep(0.01);
    offsetSpinBox->setFixedWidth(80);
    offsetSpinBox->setAlignment(Qt::AlignRight);
    connect(offsetSpinBox, SIGNAL(valueChanged(double)), glWidget, SLOT(onSetOffset(double)));

    label = new QLabel(QString("Gaussian Filter Offset:"));
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    label->setAlignment(Qt::AlignRight);
    ((QGridLayout *)(box->layout()))->addWidget(label, 1, 0, 1, 1, Qt::AlignRight);
    ((QGridLayout *)(box->layout()))->addWidget(offsetSpinBox, 1, 1, 1, 1, Qt::AlignLeft);

    mednSpinBox = new QSpinBox();
    mednSpinBox->setMinimum(0);
    mednSpinBox->setMaximum(127);
    mednSpinBox->setFixedWidth(80);
    mednSpinBox->setAlignment(Qt::AlignRight);
    connect(mednSpinBox, SIGNAL(valueChanged(int)), glWidget, SLOT(onSetMedianRadius(int)));

    label = new QLabel(QString("Median Smoother Radius:"));
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    label->setAlignment(Qt::AlignRight);
    ((QGridLayout *)(box->layout()))->addWidget(label, 1, 2, 1, 1, Qt::AlignRight);
    ((QGridLayout *)(box->layout()))->addWidget(mednSpinBox, 1, 3, 1, 1, Qt::AlignLeft);

    box = new QGroupBox(QString("CalTag Parameters"));
    box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    box->setLayout(new QGridLayout());
    box->layout()->setContentsMargins(6, 6, 6, 6);
    ((QGridLayout *)(box->layout()))->setColumnStretch(0, 100);
    ((QGridLayout *)(box->layout()))->setColumnMinimumWidth(2, 160);
    this->layout()->addWidget(box);

    minRegionArea = new QSpinBox();
    minRegionArea->setMinimum(0);
    minRegionArea->setMaximum(1000000);
    minRegionArea->setValue(128 - 32);
    minRegionArea->setFixedWidth(80);
    connect(minRegionArea, SIGNAL(valueChanged(int)), glWidget, SLOT(onSetMinRegionArea(int)));

    label = new QLabel(QString("Minimum Region Area:"));
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    label->setAlignment(Qt::AlignRight);
    ((QGridLayout *)(box->layout()))->addWidget(label, 0, 0, 1, 1, Qt::AlignRight);
    ((QGridLayout *)(box->layout()))->addWidget(minRegionArea, 0, 1, 1, 1, Qt::AlignLeft);

    maxRegionArea = new QSpinBox();
    maxRegionArea->setMinimum(0);
    maxRegionArea->setMaximum(1000000);
    maxRegionArea->setValue((640 * 480) / 16);
    maxRegionArea->setFixedWidth(80);
    connect(maxRegionArea, SIGNAL(valueChanged(int)), glWidget, SLOT(onSetMaxRegionArea(int)));

    label = new QLabel(QString("Maximum Region Area:"));
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    label->setAlignment(Qt::AlignRight);
    ((QGridLayout *)(box->layout()))->addWidget(label, 0, 2, 1, 1, Qt::AlignRight);
    ((QGridLayout *)(box->layout()))->addWidget(maxRegionArea, 0, 3, 1, 1, Qt::AlignLeft);

    minBoxCount = new QSpinBox();
    minBoxCount->setMinimum(0);
    minBoxCount->setMaximum(1000000);
    minBoxCount->setValue(128);
    minBoxCount->setFixedWidth(80);
    connect(minBoxCount, SIGNAL(valueChanged(int)), glWidget, SLOT(onSetMinBoxCount(int)));

    label = new QLabel(QString("Minimum Box Count:"));
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    label->setAlignment(Qt::AlignRight);
    ((QGridLayout *)(box->layout()))->addWidget(label, 1, 0, 1, 1, Qt::AlignRight);
    ((QGridLayout *)(box->layout()))->addWidget(minBoxCount, 1, 1, 1, 1, Qt::AlignLeft);

    flipCalTagsFlag = new QCheckBox();
    flipCalTagsFlag->setCheckable(true);
    flipCalTagsFlag->setChecked(true);
    flipCalTagsFlag->setFixedWidth(80);
    connect(flipCalTagsFlag, SIGNAL(clicked(bool)), glWidget, SLOT(onSetFlipCalTagsFlag(bool)));

    label = new QLabel(QString("Flip CalTag (backlight):"));
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    label->setAlignment(Qt::AlignRight);
    ((QGridLayout *)(box->layout()))->addWidget(label, 1, 2, 1, 1, Qt::AlignRight);
    ((QGridLayout *)(box->layout()))->addWidget(flipCalTagsFlag, 1, 3, 1, 1, Qt::AlignLeft);

    // INITIALIZE THE CURVATURE FILTER
    QSettings settings;

    // LOAD THE PARAMETERS FROM SETTINGS
    iterSpinBox->setValue(settings.value(QString("LAUCalTagScanWidget::iterSpinBox"), glWidget->iterations()).toInt());
    gausSpinBox->setValue(settings.value(QString("LAUCalTagScanWidget::gausSpinBox"), glWidget->gaussianRadius()).toInt());
    mednSpinBox->setValue(settings.value(QString("LAUCalTagScanWidget::mednSpinBox"), glWidget->medianRadius()).toInt());
    offsetSpinBox->setValue(settings.value(QString("LAUCalTagScanWidget::offsetSpinBox"), glWidget->offset()).toDouble());

    // COPY THE PARAMETERS OVER TO THE GLFILTER
    glWidget->onSetMedianRadius(mednSpinBox->value());
    glWidget->onSetGaussianRadius(gausSpinBox->value());
    glWidget->onSetIterations(iterSpinBox->value());
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUCalTagGLWidget::LAUCalTagGLWidget(QWidget *parent) : LAUVideoGLWidget(parent), displayTextureFlag(true)
{
    // INITIALIZE PRIVATE VARIABLES
    textureLUT = NULL;

    frameBufferObjectA = NULL;
    frameBufferObjectB = NULL;
    frameBufferObjectC = NULL;
    frameBufferObjectD = NULL;

    quantizationOffset = CALTAGSCANGLFILTERQUANTIZATIONOFFSET;
    medianFilterRadius = CALTAGSCANGLFILTERMEDIANFILTERRADIUS;
    gaussianFilterRadius = CALTAGSCANGLFILTERGAUSSIANFILTERRADIUS;
    iterationCount = CALTAGSCANGLFILTERITERATIONCOUNT;

    // INITIALIZE CLASS VARIABLES
    minBoxCount     = 32;                   // 2X2 PIXELS TIMES 8X8 BOX SIZE
    maxRegionArea   = (640 * 480) / 16;     // NEEDS IMAGE SIZE AND BYTES PER PIXEL
    minRegionArea   = maxRegionArea / 10;
    flipCalTagsFlag = true;
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUCalTagGLWidget::~LAUCalTagGLWidget()
{
    if (wasInitialized()) {
        makeCurrent();
        if (textureLUT) {
            delete textureLUT;
        }
        if (frameBufferObjectA) {
            delete frameBufferObjectA;
        }
        if (frameBufferObjectB) {
            delete frameBufferObjectB;
        }
        if (frameBufferObjectC) {
            delete frameBufferObjectC;
        }
        if (frameBufferObjectD) {
            delete frameBufferObjectD;
        }
    }
    qDebug() << "LAUCalTagGLWidget::~LAUCalTagGLWidget()";
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUCalTagGLWidget::testFBO(QOpenGLFramebufferObject *fbo[])
{
    if ((*fbo) == NULL) {
        // CREATE A FORMAT OBJECT FOR CREATING THE FRAME BUFFER
        QOpenGLFramebufferObjectFormat frameBufferObjectFormat;
        frameBufferObjectFormat.setInternalTextureFormat(GL_RGBA32F);

        (*fbo) = new QOpenGLFramebufferObject(videoTexture->width(), videoTexture->height(), frameBufferObjectFormat);
        (*fbo)->release();
    } else if ((*fbo)->width() != videoTexture->width() || (*fbo)->height() != videoTexture->height()) {
        delete (*fbo);

        // CREATE A FORMAT OBJECT FOR CREATING THE FRAME BUFFER
        QOpenGLFramebufferObjectFormat frameBufferObjectFormat;
        frameBufferObjectFormat.setInternalTextureFormat(GL_RGBA32F);

        (*fbo) = new QOpenGLFramebufferObject(videoTexture->width(), videoTexture->height(), frameBufferObjectFormat);
        (*fbo)->release();
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUCalTagGLWidget::initialize()
{
    LAUVideoGLWidget::initialize();

    // CREATE A TEXTURE FOR LOOK UP OPERATIONS
    textureLUT = new QOpenGLTexture(QOpenGLTexture::Target1D);
    textureLUT->setSize(512);
    textureLUT->setFormat(QOpenGLTexture::RGBA32F);
    textureLUT->setWrapMode(QOpenGLTexture::ClampToBorder);
    textureLUT->setMinificationFilter(QOpenGLTexture::Nearest);
    textureLUT->setMagnificationFilter(QOpenGLTexture::Nearest);
    textureLUT->allocateStorage();

    // NOW ADD OUR LIST OF HARRIS CORNER SHADER PROGRAMS
    setlocale(LC_NUMERIC, "C");
    programA.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/Shaders/filterRGBAtoGray.vert");
    programA.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Shaders/filterRGBAtoGray.frag");
    programA.link();

    programB.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/Shaders/filterXGaussian.vert");
    programB.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Shaders/filterXGaussian.frag");
    programB.link();

    programC.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/Shaders/filterYGaussian.vert");
    programC.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Shaders/filterYGaussian.frag");
    programC.link();

    programD.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/Shaders/filterAdaptiveThreshold.vert");
    programD.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Shaders/filterAdaptiveThreshold.frag");
    programD.link();

    programE.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/Shaders/filterBinaryMedian.vert");
    programE.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Shaders/filterBinaryMedian.frag");
    programE.link();

    programF.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/Shaders/filterSobelEdge.vert");
    programF.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Shaders/filterSobelEdge.frag");
    programF.link();

    programG.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/Shaders/filterBinaryLookUpTable.vert");
    programG.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Shaders/filterBinaryLookUpTable.frag");
    programG.link();

    programH.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/Shaders/filterMaxRGBA.vert");
    programH.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Shaders/filterMaxRGBA.frag");
    programH.link();

    programI.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/Shaders/filterMinRGBA.vert");
    programI.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Shaders/filterMinRGBA.frag");
    programI.link();

    programJ.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/Shaders/calTagXYZW.vert");
    programJ.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Shaders/calTagXYZW.frag");
    programJ.link();

    programK.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/Shaders/displayCalTag.vert");
    programK.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Shaders/displayCalTag.frag");
    programK.link();
    setlocale(LC_ALL, "");
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUCalTagGLWidget::process()
{
    if (videoTexture) {
        // SEE IF WE NEED NEW FBOS
        testFBO(&frameBufferObjectA);
        testFBO(&frameBufferObjectB);
        testFBO(&frameBufferObjectC);
        testFBO(&frameBufferObjectD);

        // RESIZE THE BYTE ARRAYS AS NEEDED
        memoryObject[0].resize(videoTexture->width()*videoTexture->height());
        memoryObject[1].resize(videoTexture->width()*videoTexture->height());
        debugObject.resize(3 * videoTexture->width()*videoTexture->height());

        // BINARIZE THE INCOMING BUFFER
        binarize(frameBufferObjectA, frameBufferObjectB, frameBufferObjectC);
        sobel(frameBufferObjectC, frameBufferObjectB);
        cleanUp(frameBufferObjectB, frameBufferObjectA);
        dilationErosion(frameBufferObjectA, frameBufferObjectB);

        // DOWNLOAD THE RESULTING BINARY TEXTURE TO OUR MEMORY BUFFER FOR FURTHER PROCESSING
        glBindTexture(GL_TEXTURE_2D, frameBufferObjectB->texture());
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, (unsigned char *)memoryObject[0].constData());

        glBindTexture(GL_TEXTURE_2D, frameBufferObjectC->texture());
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, (unsigned char *)memoryObject[1].constData());
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *)debugObject.constData());

        // LOOK FOR CALTAGS AND GET THE CR TO XYZ TRANSFORM
        bool okay = false;
        cv::Mat transform = detectCalTagGrid(&okay);
        float transformAsVector[30];
        for (int n = 0; n < 30; n++) {
            transformAsVector[n] = (float)transform.at<double>(n);
        }

        qDebug() << "Detected grid:" << okay;

        // UPLOAD THE DEBUG BUFFER TO THE GPU AS A TEXTURE
        videoTexture->setData(QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)debugObject.constData());

        // DRAW THE BEST-FIT XY PLANE TO SCREEN WITH THE BINARY IMAGE
        if (frameBufferObjectB && frameBufferObjectB->bind()) {
            if (programJ.bind()) {
                // CLEAR THE FRAME BUFFER OBJECT
                glViewport(0, 0, frameBufferObjectB->width(), frameBufferObjectB->height());
                glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
                if (quadVertexBuffer.bind()) {
                    if (quadIndexBuffer.bind()) {
                        // BIND THE TEXTURE FROM THE FRAME BUFFER OBJECT
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, frameBufferObjectC->texture());
                        programJ.setUniformValue("qt_texture", 0);

                        // SET THE TRANSFORM PARAMETERS ON THE SHADER
                        programJ.setUniformValue("qt_offset", QPointF((float)frameBufferObjectB->width() / 2.0f, (float)frameBufferObjectB->height() / 2.0f));
                        programJ.setUniformValueArray("qt_transform", transformAsVector, 30, 1);

                        // TELL OPENGL PROGRAMMABLE PIPELINE HOW TO LOCATE VERTEX POSITION DATA
                        glVertexAttribPointer(programJ.attributeLocation("qt_vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
                        programJ.enableAttributeArray("qt_vertex");
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                        // RELEASE THE FRAME BUFFER OBJECT AND ITS ASSOCIATED GLSL PROGRAMS
                        quadIndexBuffer.release();
                    }
                    quadVertexBuffer.release();
                }
                programJ.release();
            }
            frameBufferObjectB->release();
        }
    }
}


/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUCalTagGLWidget::paint()
{
    if (frameBufferObjectB == NULL) {
        LAUVideoGLWidget::paint();
    } else {
        // SET THE VIEW PORT AND CLEAR THE SCREEN BUFFER
        glViewport(0, 0, localWidth, localHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // DISPLAY THE LAST FBO IN OUR LIST
        if (videoTexture) {
            if (programK.bind()) {
                if (quadVertexBuffer.bind()) {
                    if (quadIndexBuffer.bind()) {
                        // SET THE ACTIVE TEXTURE ON THE GPU
                        glActiveTexture(GL_TEXTURE0);
                        //videoTexture->bind();
                        glBindTexture(GL_TEXTURE_2D, frameBufferObjectB->texture());
                        programK.setUniformValue("qt_texture", 0);

                        if (displayTextureFlag) {
                            programK.setUniformValue("qt_mode", 0);
                        } else {
                            programK.setUniformValue("qt_mode", 1);
                        }

                        // TELL OPENGL PROGRAMMABLE PIPELINE HOW TO LOCATE VERTEX POSITION DATA
                        programK.setAttributeBuffer("qt_vertex", GL_FLOAT, 0, 4, 4 * sizeof(float));
                        programK.enableAttributeArray("qt_vertex");

                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                        quadIndexBuffer.release();
                    }
                    quadVertexBuffer.release();
                }
                programK.release();
            }
        }
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUCalTagGLWidget::dilationErosion(QOpenGLFramebufferObject *fboA, QOpenGLFramebufferObject *fboB)
{
    // APPLY EROSION AS THE MIN FILTER OPERATION
    if (fboB && fboB->bind()) {
        if (programG.bind()) {
            // CLEAR THE FRAME BUFFER OBJECT
            glViewport(1, 1, fboB->width() - 2, fboB->height() - 2);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
            if (quadVertexBuffer.bind()) {
                if (quadIndexBuffer.bind()) {
                    // BIND THE TEXTURE FROM THE FRAME BUFFER OBJECT
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, fboA->texture());
                    programG.setUniformValue("qt_texture", 0);

                    // SET THE MEDIAN FILTER RADIUS
                    programG.setUniformValue("qt_radius", 1);

                    // TELL OPENGL PROGRAMMABLE PIPELINE HOW TO LOCATE VERTEX POSITION DATA
                    glVertexAttribPointer(programG.attributeLocation("qt_vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
                    programG.enableAttributeArray("qt_vertex");
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                    // RELEASE THE FRAME BUFFER OBJECT AND ITS ASSOCIATED GLSL PROGRAMS
                    quadIndexBuffer.release();
                }
                quadVertexBuffer.release();
            }
            programG.release();
        }
        fboB->release();
    }

    // APPLY DILATION AS THE MAX FILTER OPERATION
    if (fboA && fboA->bind()) {
        if (programI.bind()) {
            // CLEAR THE FRAME BUFFER OBJECT
            glViewport(1, 1, fboA->width() - 2, fboA->height() - 2);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
            if (quadVertexBuffer.bind()) {
                if (quadIndexBuffer.bind()) {
                    // BIND THE TEXTURE FROM THE FRAME BUFFER OBJECT
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, fboB->texture());
                    programI.setUniformValue("qt_texture", 0);

                    // SET THE MEDIAN FILTER RADIUS
                    programI.setUniformValue("qt_radius", 1);

                    // TELL OPENGL PROGRAMMABLE PIPELINE HOW TO LOCATE VERTEX POSITION DATA
                    glVertexAttribPointer(programI.attributeLocation("qt_vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
                    programI.enableAttributeArray("qt_vertex");
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                    // RELEASE THE FRAME BUFFER OBJECT AND ITS ASSOCIATED GLSL PROGRAMS
                    quadIndexBuffer.release();
                }
                quadVertexBuffer.release();
            }
            programI.release();
        }
        fboA->release();
    }

    // COPY THE RESCALED IMAGE BACK TO FRAME BUFFER OBJECT B
    QOpenGLFramebufferObject::blitFramebuffer(fboA, fboB);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUCalTagGLWidget::cleanUp(QOpenGLFramebufferObject *fboA, QOpenGLFramebufferObject *fboB)
{
    for (int n = 0; n < 4; n++) {
        QOpenGLFramebufferObject *inFBO = NULL;
        QOpenGLFramebufferObject *otFBO = NULL;

        switch (n) {
            case 0:
                inFBO = fboA;
                otFBO = fboB;
                textureLUT->setData(QOpenGLTexture::Red, QOpenGLTexture::UInt8, (const void *)bridgeLutA);
                break;
            case 1:
                inFBO = fboA;
                otFBO = fboB;
                textureLUT->setData(QOpenGLTexture::Red, QOpenGLTexture::UInt8, (const void *)bridgeLutB);
                break;
            case 2:
                inFBO = fboB;
                otFBO = fboA;
                textureLUT->setData(QOpenGLTexture::Red, QOpenGLTexture::UInt8, (const void *)bridgeLutC);
                break;
            case 3:
                inFBO = fboA;
                otFBO = fboB;
                textureLUT->setData(QOpenGLTexture::Red, QOpenGLTexture::UInt8, (const void *)bridgeLutD);
                break;
        }

        // APPLY A BRIDGE BINARY FILTER TO THE BINARY IMAGE
        if (otFBO && otFBO->bind()) {
            if (programG.bind()) {
                // CLEAR THE FRAME BUFFER OBJECT
                glViewport(1, 1, otFBO->width() - 2, otFBO->height() - 2);
                glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
                if (quadVertexBuffer.bind()) {
                    if (quadIndexBuffer.bind()) {
                        // BIND THE TEXTURE FROM THE FRAME BUFFER OBJECT
                        glActiveTexture(GL_TEXTURE6);
                        glBindTexture(GL_TEXTURE_2D, inFBO->texture());
                        programG.setUniformValue("qt_texture", 6);

                        // BIND THE TEXTURE FROM THE FRAME BUFFER OBJECT
                        glActiveTexture(GL_TEXTURE7);
                        textureLUT->bind();
                        programG.setUniformValue("qt_lookUpTable", 7);

                        // TELL OPENGL PROGRAMMABLE PIPELINE HOW TO LOCATE VERTEX POSITION DATA
                        glVertexAttribPointer(programG.attributeLocation("qt_vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
                        programG.enableAttributeArray("qt_vertex");
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                        // RELEASE THE FRAME BUFFER OBJECT AND ITS ASSOCIATED GLSL PROGRAMS
                        quadIndexBuffer.release();
                    }
                    quadVertexBuffer.release();
                }
                programG.release();
            }
            otFBO->release();
        }

        if (n == 0 && inFBO && inFBO->bind()) {
            if (programE.bind()) {
                // CLEAR THE FRAME BUFFER OBJECT
                glViewport(0, 0, inFBO->width(), inFBO->height());
                glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
                if (quadVertexBuffer.bind()) {
                    if (quadIndexBuffer.bind()) {
                        // BIND THE TEXTURE FROM THE FRAME BUFFER OBJECT
                        glActiveTexture(GL_TEXTURE4);
                        glBindTexture(GL_TEXTURE_2D, otFBO->texture());
                        programE.setUniformValue("qt_texture", 4);

                        // SET THE MEDIAN FILTER RADIUS
                        programE.setUniformValue("qt_radius", 1);
                        programE.setUniformValue("qt_threshold", 4.5f);

                        // Tell OpenGL programmable pipeline how to locate vertex position data
                        glVertexAttribPointer(programE.attributeLocation("qt_vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
                        programE.enableAttributeArray("qt_vertex");
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                        // RELEASE THE FRAME BUFFER OBJECT AND ITS ASSOCIATED GLSL PROGRAMS
                        quadIndexBuffer.release();
                    }
                    quadVertexBuffer.release();
                }
                programE.release();
            }
            inFBO->release();
        }
    }

    // COPY THE RESCALED IMAGE BACK TO FRAME BUFFER OBJECT B
    QOpenGLFramebufferObject::blitFramebuffer(fboB, fboA);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUCalTagGLWidget::sobel(QOpenGLFramebufferObject *fboA, QOpenGLFramebufferObject *fboB)
{
    // APPLY A SOBEL EDGE DETECTING FILTER TO THE BINARY IMAGE
    if (fboB && fboB->bind()) {
        if (programF.bind()) {
            // CLEAR THE FRAME BUFFER OBJECT
            glViewport(0, 0, fboB->width(), fboB->height());
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
            if (quadVertexBuffer.bind()) {
                if (quadIndexBuffer.bind()) {
                    // BIND THE TEXTURE FROM THE FRAME BUFFER OBJECT
                    glActiveTexture(GL_TEXTURE5);
                    glBindTexture(GL_TEXTURE_2D, fboA->texture());
                    programF.setUniformValue("qt_texture", 5);

                    // TELL OPENGL PROGRAMMABLE PIPELINE HOW TO LOCATE VERTEX POSITION DATA
                    glVertexAttribPointer(programF.attributeLocation("qt_vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
                    programF.enableAttributeArray("qt_vertex");
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                    // RELEASE THE FRAME BUFFER OBJECT AND ITS ASSOCIATED GLSL PROGRAMS
                    quadIndexBuffer.release();
                }
                quadVertexBuffer.release();
            }
            programF.release();
        }
        fboB->release();
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUCalTagGLWidget::binarize(QOpenGLFramebufferObject *fboA, QOpenGLFramebufferObject *fboB, QOpenGLFramebufferObject *fboC)
{
    // BIND THE FRAME BUFFER OBJECT FOR PROCESSING
    if (fboA && fboA->bind()) {
        if (programA.bind()) {
            // CLEAR THE FRAME BUFFER OBJECT
            glViewport(0, 0, fboA->width(), fboA->height());
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
            if (quadVertexBuffer.bind()) {
                if (quadIndexBuffer.bind()) {
                    // BIND THE TEXTURE FROM THE FRAME BUFFER OBJECT
                    glActiveTexture(GL_TEXTURE0);
                    videoTexture->bind();
                    programA.setUniformValue("qt_texture", 0);

                    // Tell OpenGL programAmable pipeline how to locate vertex position data
                    glVertexAttribPointer(programA.attributeLocation("qt_vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
                    programA.enableAttributeArray("qt_vertex");
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                    // RELEASE THE FRAME BUFFER OBJECT AND ITS ASSOCIATED GLSL PROGRAMS
                    quadIndexBuffer.release();
                }
                quadVertexBuffer.release();
            }
            programA.release();
        }
        fboA->release();
    }

    // COPY THE RESCALED IMAGE BACK TO FRAME BUFFER OBJECT A TO FBO C
    QOpenGLFramebufferObject::blitFramebuffer(fboC, fboA);

    for (unsigned int iter = 0; iter < iterationCount; iter++) {
        // APPLY A GAUSSIAN FILTER IN THE X-DIRECTION TO THE FBOC TO FBOD
        if (fboB && fboB->bind()) {
            if (programB.bind()) {
                // CLEAR THE FRAME BUFFER OBJECT
                glViewport(0, 0, fboB->width(), fboB->height());
                glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
                if (quadVertexBuffer.bind()) {
                    if (quadIndexBuffer.bind()) {
                        // BIND THE TEXTURE FROM THE FRAME BUFFER OBJECT
                        glActiveTexture(GL_TEXTURE1);
                        glBindTexture(GL_TEXTURE_2D, fboC->texture());
                        programB.setUniformValue("qt_texture", 1);

                        // TELL THE SHADER HOW MANY PIXELS WIDE TO APPLY GAUSSIAN FILTER
                        programB.setUniformValue("qt_width", (int)gaussianFilterRadius);

                        // TELL OPENGL PROGRAMMABLE PIPELINE HOW TO LOCATE VERTEX POSITION DATA
                        glVertexAttribPointer(programB.attributeLocation("qt_vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
                        programB.enableAttributeArray("qt_vertex");
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                        // RELEASE THE FRAME BUFFER OBJECT AND ITS ASSOCIATED GLSL PROGRAMS
                        quadIndexBuffer.release();
                    }
                    quadVertexBuffer.release();
                }
                programB.release();
            }
            fboB->release();
        }

        // APPLY A GAUSSIAN FILTER IN THE Y-DIRECTION TO THE FBOD TO FBOC
        if (fboC && fboC->bind()) {
            if (programC.bind()) {
                // CLEAR THE FRAME BUFFER OBJECT
                glViewport(0, 0, fboC->width(), fboC->height());
                glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
                if (quadVertexBuffer.bind()) {
                    if (quadIndexBuffer.bind()) {
                        // BIND THE TEXTURE FROM THE FRAME BUFFER OBJECT
                        glActiveTexture(GL_TEXTURE2);
                        glBindTexture(GL_TEXTURE_2D, fboB->texture());
                        programC.setUniformValue("qt_texture", 2);

                        // TELL THE SHADER HOW MANY PIXELS WIDE TO APPLY GAUSSIAN FILTER
                        programC.setUniformValue("qt_width", (int)gaussianFilterRadius);

                        // Tell OpenGL programmable pipeline how to locate vertex position data
                        glVertexAttribPointer(programC.attributeLocation("qt_vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
                        programC.enableAttributeArray("qt_vertex");
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                        // RELEASE THE FRAME BUFFER OBJECT AND ITS ASSOCIATED GLSL PROGRAMS
                        quadIndexBuffer.release();
                    }
                    quadVertexBuffer.release();
                }
                programC.release();
            }
            fboC->release();
        }
    }

    // APPLY THRESHOLD OF FBOA BY SMOOTHED IMAGE FBOC
    if (fboB && fboB->bind()) {
        if (programD.bind()) {
            // CLEAR THE FRAME BUFFER OBJECT
            glViewport(0, 0, fboB->width(), fboB->height());
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
            if (quadVertexBuffer.bind()) {
                if (quadIndexBuffer.bind()) {
                    // BIND THE TEXTURE FROM THE FRAME BUFFER OBJECT
                    glActiveTexture(GL_TEXTURE3);
                    glBindTexture(GL_TEXTURE_2D, fboA->texture());
                    programD.setUniformValue("qt_texture", 3);

                    // BIND THE THRESHOLD FROM THE FRAME BUFFER OBJECT
                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_2D, fboC->texture());
                    programD.setUniformValue("qt_threshold", 1);
                    programD.setUniformValue("qt_invert", false);

                    // SET THE FLOATING POINT OFFSET FOR QUANTIZATION
                    programD.setUniformValue("qt_offset", quantizationOffset);

                    // Tell OpenGL programmable pipeline how to locate vertex position data
                    glVertexAttribPointer(programD.attributeLocation("qt_vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
                    programD.enableAttributeArray("qt_vertex");
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                    // RELEASE THE FRAME BUFFER OBJECT AND ITS ASSOCIATED GLSL PROGRAMS
                    quadIndexBuffer.release();
                }
                quadVertexBuffer.release();
            }
            programD.release();
        }
        fboB->release();
    }

    // APPLY A MEDIAN FILTER TO THE BINARY IMAGE
    if (medianFilterRadius > 0) {
        if (fboC && fboC->bind()) {
            if (programE.bind()) {
                // CLEAR THE FRAME BUFFER OBJECT
                glViewport(0, 0, fboC->width(), fboC->height());
                glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
                if (quadVertexBuffer.bind()) {
                    if (quadIndexBuffer.bind()) {
                        // BIND THE TEXTURE FROM THE FRAME BUFFER OBJECT
                        glActiveTexture(GL_TEXTURE4);
                        glBindTexture(GL_TEXTURE_2D, fboB->texture());
                        programE.setUniformValue("qt_texture", 4);

                        // SET THE MEDIAN FILTER RADIUS
                        programE.setUniformValue("qt_radius", (int)medianFilterRadius);
                        programE.setUniformValue("qt_threshold", (float)((2 * medianFilterRadius + 1) * (2 * medianFilterRadius + 1)) / 2.0f);

                        // Tell OpenGL programmable pipeline how to locate vertex position data
                        glVertexAttribPointer(programE.attributeLocation("qt_vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
                        programE.enableAttributeArray("qt_vertex");
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                        // RELEASE THE FRAME BUFFER OBJECT AND ITS ASSOCIATED GLSL PROGRAMS
                        quadIndexBuffer.release();
                    }
                    quadVertexBuffer.release();
                }
                programE.release();
            }
            fboC->release();
        }
    } else {
        // COPY THE RESCALED IMAGE BACK TO FRAME BUFFER OBJECT A TO FBO C
        QOpenGLFramebufferObject::blitFramebuffer(fboC, fboB);
    }
}

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
cv::Mat LAUCalTagGLWidget::detectCalTagGrid(bool *okay)
{
    // CLEAR THE GRID PAIRINGS FROM ANY PREVIOUS CALL
    gridPairings.clear();

    // CREATE A RETURN MATRIX
    cv::Mat localTransform(1, 30, CV_64F);

    // CREATE CV::MAT WRAPPER AROUND OUR MEMORY OBJECT
    cv::Mat sbImage(videoTexture->height(), videoTexture->width(), CV_8UC1, (unsigned char *)memoryObject[0].constData(), videoTexture->width());
    cv::Mat inImage(videoTexture->height(), videoTexture->width(), CV_8UC1, (unsigned char *)memoryObject[1].constData(), videoTexture->width());
    cv::Mat dbImage(videoTexture->height(), videoTexture->width(), CV_8UC3, (unsigned char *)debugObject.constData(), 3 * videoTexture->width());

    cv::vector<cv::RotatedRect> rotatedRects = regionArea(sbImage);

    // MAKE SURE WE HAVE ENOUGH DETECTED RECTANGLES
    if (rotatedRects.size() > (unsigned long)minBoxCount) {
        // GET A GOOD APPROXIMATION OF WHERE THE SADDLE POINTS ARE
        cv::vector<cv::vector<cv::Point2f>> squares = findSaddles(rotatedRects);

        for (unsigned int n = 0; n < squares.size(); n++) {
            for (unsigned int m = 0; m < squares[n].size(); m++) {
                cv::circle(dbImage, squares[n][m], 2, cv::Scalar(0, 0, 255), 2);
            }
        }

        // DECODE THE CALTAG SQUARES
        cv::vector<cv::vector<cv::Point2f>> coordinates = findPattern(inImage, squares);

        // DELETE INVALID SQUARES WHILE ACCUMULATING POINTS FROM IMAGE TO GRID SPACE
        cv::vector<cv::Point2f> toPoints, fmPoints;
        for (unsigned int i = 0; i < coordinates.size(); i++) {
            for (unsigned int j = 0; j < 4; j++) {
                if (coordinates[i][j] == coordinates[i][j]) {
                    // FILTER POINTS LISTS TO ELIMINATE DUPLICATES
                    bool flag = true;
                    for (int n = 0; n < (int)fmPoints.size(); n++) {
                        if (fmPoints[n] == squares[i][j]) {
                            flag = false;
                        }
                    }
                    if (flag) {
                        fmPoints.push_back(squares[i][j]);
                        toPoints.push_back(coordinates[i][j]);
                    }
                }
            }
        }

        // LETS REMOVE OUTLIERS FROM OUR FMPOINTS BY USING A LINEAR MAPPING BETWEEN THE TWO
        toPoints = removeOutlierPoints(fmPoints, toPoints);

        // MAKE SURE WE HAVE ENOUGH DETECTED POINTS
        if (toPoints.size() > (unsigned long)minBoxCount) {
            // MAKE A LOCAL COPY OF THE POINT PAIRINGS
            for (unsigned int n = 0; n < toPoints.size() && n < fmPoints.size(); n++) {
                Pairing pairing;
                pairing.cr = QPoint(fmPoints[n].x, fmPoints[n].y);
                pairing.xy = QPoint(toPoints[n].x, toPoints[n].y);
                gridPairings << pairing;
            }

            // FIND BEST TRANSFORM FROM CAMERA TO CALIBRATION WORLD COORDINATES
            localTransform = findBestQuadraticMapping(fmPoints, toPoints, CALTAGSCANGLFILTERBESTFITTRANSFORMPOLYNOMIALORDER);
            if (okay) {
                *okay = true;
            }
        } else {
            // SET THE TRANSFORM TO AN INVALID MATRIX
            localTransform.setTo(NAN);
            if (okay) {
                *okay = false;
            }
        }
    } else {
        // SET THE TRANSFORM TO AN INVALID MATRIX
        localTransform.setTo(NAN);
        if (okay) {
            *okay = false;
        }
    }
    return (localTransform);
}

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
cv::Mat LAUCalTagGLWidget::cleanStrays(cv::Mat inImage)
{
    // MAKE A COPY OF THE INPUT IMAGE TO BE SAME FORMAT AND SIZE
    cv::Mat otImage = inImage;

    //REMOVE RANDOM NOISE THAT WAS IDENTIFIED AS A CONTOUR
    cv::vector<cv::vector<cv::Point>> contours;
    cv::vector<cv::Vec4i> hierarchy;

    cv::findContours(inImage.clone(), contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_NONE, cv::Point(0, 0));
    cv::Scalar color = cv::Scalar(0);
    for (unsigned int i = 0; i < contours.size(); i++) {
        if (contours[i].size() < 8) {
            cv::floodFill(otImage, contours[i][0], color);
        }
    }
    return (otImage);
}

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
cv::vector<cv::RotatedRect> LAUCalTagGLWidget::regionArea(cv::Mat inImage)
{
    // CREATE ASSOCIATED DATA STRUCTURES TO HOLD INTERMEDIATE RESULTS
    cv::vector<cv::vector<cv::Point>> contours;
    cv::vector<cv::Vec4i> hierarchy;
    cv::vector<cv::RotatedRect> rotatedRects;

    // CREATE A DEBUG IMAGE TO DRAW INTO
    cv::Mat dbImage(videoTexture->height(), videoTexture->width(), CV_8UC3, (unsigned char *)debugObject.constData(), 3 * videoTexture->width());

    // SEARCH AND STORE ANY CONTOURS IN THE IMAGE
    cv::findContours(inImage.clone(), contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE, cv::Point(0, 0));

    float mean = 0.0f;
    int increment = 0;
    float sum = 0.0f;

    cv::vector<cv::vector<cv::Point>> contoursPoly;
    cv::vector<cv::RotatedRect> rectangles;

    // FOR EACH CONTOUR, APPROXIMATE IT WITH A POLYGON
    for (unsigned int i = 0; i < contours.size(); ++i) {
        contoursPoly.push_back(contours[i]);
        cv::approxPolyDP(contours[i], contoursPoly[i], 3, true);
    }

    // USE THE HEIRARCHY FROM FINDCONTOURS TO NARROW THE NUMBER OF POSSIBLE REGIONS
    for (cv::vector<cv::Vec4i>::size_type idx = 0; idx < hierarchy.size(); ++idx) {
        if ((hierarchy[idx][0] != -1) && (hierarchy[idx][3] != -1)) {
            rectangles.push_back(cv::minAreaRect(contoursPoly[idx]));
#ifdef DONTCOMPILE
            // DRAW ALL THE ROTATED RECTANGLES AT THIS POINT
            cv::Point2f rect_points[4];
            cv::minAreaRect(contoursPoly[idx]).points(rect_points);
            for (int j = 0; j < 4; j++) {
                cv::line(dbImage, rect_points[j], rect_points[(j + 1) % 4], cv::Scalar(0, 255, 0));
            }
#endif
        }
    }
#ifdef DONTCOMPILE
    // DRAW ALL THE DETECTED CONTOURS AT THIS POINT
    cv::drawContours(dbImage, contoursPoly, -1, cv::Scalar(255, 0, 0));
#endif

    // USE THE MINIMUM AND MAXIMUM REGIONS TO ELIMINATE MORE BOXES
    cv::vector<cv::RotatedRect> bestRectangles;
    cv::vector<float> areas;
    for (unsigned int i = 0; i < rectangles.size(); ++i) {
        cv::Point2f vertices[4];
        rectangles[i].points(vertices);
        if ((rectangles[i].size.width * rectangles[i].size.height) > minRegionArea) {
            if ((rectangles[i].size.width * rectangles[i].size.height) < maxRegionArea) {
                if (rectangles[i].size.width >= (0.8 * rectangles[i].size.height)) {
                    if (rectangles[i].size.width <= (1.2 * rectangles[i].size.height)) {
                        bestRectangles.push_back(rectangles[i]);
                        for (int j = 0; j < 4; j++) {
                            areas.push_back((float)cv::norm(vertices[j] - vertices[(j + 1) % 4]));
                            sum += cv::norm(vertices[j] - vertices[(j + 1) % 4]);
                            increment++;
                        }

                        cv::Point2f rect_points[4];
                        rectangles[i].points(rect_points);
                        for (int j = 0; j < 4; j++) {
                            cv::line(dbImage, rect_points[j], rect_points[(j + 1) % 4], cv::Scalar(255, 0, 0));
                        }
                    }
                }
            }
        }
    }
    mean = sum * (1.0f / increment);

    // SORT THE AREAS TO FIND THE MEDIAN
    for (unsigned int n = 0; n < areas.size(); n++) {
        for (unsigned int m = n + 1; m < areas.size(); m++) {
            if (areas[m] < areas[n]) {
                float temp = areas[m];
                areas[m] = areas[n];
                areas[n] = temp;
            }
        }
        mean = areas[areas.size() / 2];
    }

    // AVERAGE THE AREAS OF ALL BOXES AND SELECT ALL THE BOXES THAT FILL AN AVERAGE+/-10% AREA
    // THIS EFFECTIVELY LEAVES ONLY CHECKERBOARD BOXES
    for (unsigned int i = 0; i < bestRectangles.size(); ++i) {
        if (bestRectangles[i].size.width * bestRectangles[i].size.height > 0.75 * mean * mean) {
            if (bestRectangles[i].size.width * bestRectangles[i].size.height < 1.25 * mean * mean) {
                rotatedRects.push_back(bestRectangles[i]);

                // DRAW ALL THE FINAL ROTATED RECTANGLES AT THIS POINT
                cv::Point2f rect_points[4];
                bestRectangles[i].points(rect_points);
                for (int j = 0; j < 4; j++) {
                    cv::line(dbImage, rect_points[j], rect_points[(j + 1) % 4], cv::Scalar(0, 255, 0));
                }
            }
        }
    }

    // DRAW THE DEBUG BUFFER ON SCREEN FOR THE USER
    return (rotatedRects);
}

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
cv::vector<cv::vector<cv::Point2f>> LAUCalTagGLWidget::findSaddles(cv::vector<cv::RotatedRect> rotatedRects)
{
    // WE HAVE THE INDICES FOR THE CHECKERBOARD REGIONS, BUT SOME OF THEM ARE SHARED.
    // WE WANT A COMPREHENSIVE LIST OF ALL THE SADDLE POINTS, BUT WITHOUT REPEATS.
    // WE WILL DO THIS BY STORING EACH SADDLE POINT ONCE, AND ALSO BY RECORDING THE INDICES
    // WITHIN OUR VECTOR THAT ARE THE VERTICES OF EACH SQUARE
    cv::vector<cv::Point2f> corners;

    cv::vector<cv::vector<cv::Point2f>> saddleTrials;
    cv::vector<cv::vector<cv::Point2f>> squares;
    cv::vector<cv::vector<int>> squareIndices;

    for (unsigned int i = 0; i < rotatedRects.size(); i++) {
        cv::vector<int> indices;
        squareIndices.push_back(indices);
    }

    // GO AHEAD AND CHOOSE THE FIRST SQUARE AND RECORD THE INDICES.
    squareIndices[0].push_back(0);
    squareIndices[0].push_back(1);
    squareIndices[0].push_back(2);
    squareIndices[0].push_back(3);

    cv::vector<int> cornerCounter;
    for (int i = 0; i < 4; i++) {
        cornerCounter.push_back(1);
    }

    cv::Point2f vertex[4];
    cv::vector<cv::Point2f> temp;
    rotatedRects[0].points(vertex);
    for (int i = 0; i < 4; i++) {
        temp.clear();
        temp.push_back(vertex[i]);
        saddleTrials.push_back(temp);
    }

    // CHECK EACH VERTEX TO SEE IF IT IS "NEAR" ANOTHER VERTEX THATS BEEN ACCOUNTED FOR
    // IF ITS NEW, PUSH BACK ANOTHER INDICE VECTOR, IF ITS SIMILAR, ADD ANOTHER INDICE TO
    // THE RIGHT SPOT
    for (unsigned int i = 1; i < rotatedRects.size(); i++) {
        cv::Point2f vertices[4];
        rotatedRects[i].points(vertices);
        for (unsigned int j = 0; j < 4; j++) {
            size_t size = saddleTrials.size();
            for (unsigned int k = 0; k < size; k++) {
                cv::Point2f meanPoint;
                cv::Point2f sum = cv::Point2f(0.0f, 0.0f);
                for (unsigned int l = 0; l < saddleTrials[k].size(); l++) {
                    sum += saddleTrials[k][l];
                }
                meanPoint = (sum * (1.0f / saddleTrials[k].size()));
                if (sqrt(pow(vertices[j].x - meanPoint.x, 2) + pow(vertices[j].y - meanPoint.y, 2)) < 10) {
                    saddleTrials[k].push_back(vertices[j]);
                    squareIndices[i].push_back(k);
                    cornerCounter[k] += 1;
                    break;
                } else if (k == (size - 1)) {
                    temp.clear();
                    temp.push_back(vertices[j]);
                    saddleTrials.push_back(temp);
                    squareIndices[i].push_back(k + 1);
                    cornerCounter.push_back(1);
                }
            }
        }
    }

    // TAKE THE SADDLES WE HAVE FOUND AND AVERAGE THEM TO GET A FAIRLY GOOD APPROXIMATE LOCATION
    for (unsigned int k = 0; k < saddleTrials.size(); k++) {
        cv::Point2f meanPoint;
        cv::Point2f sum = cv::Point2f(0.0f, 0.0f);
        for (unsigned int l = 0; l < saddleTrials[k].size(); l++) {
            sum += saddleTrials[k][l];
        }
        meanPoint = (sum * (1.0f / saddleTrials[k].size()));
        corners.push_back(meanPoint);
    }

    // CREATE A LIST OF SQUARES THAT CONNECT THE SADDLE POINTS
    for (unsigned int k = 0; k < squareIndices.size(); k++) {
        cv::vector<cv::Point2f> square;
        for (unsigned int l = 0; l < squareIndices[k].size(); l++) {
            int index = squareIndices[k][l];
            cv::Point2f point = corners[index];
            square.push_back(point);
        }
        squares.push_back(square);
    }

    return (squares);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
cv::vector<cv::vector<cv::Point2f>> LAUCalTagGLWidget::findPattern(cv::Mat image, cv::vector<cv::vector<cv::Point2f>> squares)
{
    cv::vector<cv::vector<cv::Point2f>> outputSquares;

    // MAKE LIST OF SQUARE SPACE COORDINATES FOR A SINGLE SQUARE
    cv::vector<cv::Point2f> sqPoints;
    sqPoints.push_back(cv::Point2f(0.0f, 0.0f));
    sqPoints.push_back(cv::Point2f(1.0f, 0.0f));
    sqPoints.push_back(cv::Point2f(1.0f, 1.0f));
    sqPoints.push_back(cv::Point2f(0.0f, 1.0f));

    // MAKE LIST OF SQUARE SPACE COORDINATES FOR CODED BITS
    cv::vector<cv::Point3f> gdPoints, kgPoints;
    gdPoints.push_back(cv::Point3f(0.6875f, 0.6875f, 1.0f));
    gdPoints.push_back(cv::Point3f(0.6875f, 0.5625f, 1.0f));
    gdPoints.push_back(cv::Point3f(0.6875f, 0.4375f, 1.0f));
    gdPoints.push_back(cv::Point3f(0.6875f, 0.3125f, 1.0f));

    gdPoints.push_back(cv::Point3f(0.5625f, 0.6875f, 1.0f));
    gdPoints.push_back(cv::Point3f(0.5625f, 0.5625f, 1.0f));
    gdPoints.push_back(cv::Point3f(0.5625f, 0.4375f, 1.0f));
    gdPoints.push_back(cv::Point3f(0.5625f, 0.3125f, 1.0f));

    gdPoints.push_back(cv::Point3f(0.4375f, 0.6875f, 1.0f));
    gdPoints.push_back(cv::Point3f(0.4375f, 0.5625f, 1.0f));
    gdPoints.push_back(cv::Point3f(0.4375f, 0.4375f, 1.0f));
    gdPoints.push_back(cv::Point3f(0.4375f, 0.3125f, 1.0f));

    gdPoints.push_back(cv::Point3f(0.3125f, 0.6875f, 1.0f));
    gdPoints.push_back(cv::Point3f(0.3125f, 0.5625f, 1.0f));
    gdPoints.push_back(cv::Point3f(0.3125f, 0.4375f, 1.0f));
    gdPoints.push_back(cv::Point3f(0.3125f, 0.3125f, 1.0f));

    // CREATE DECODING MATRIX FOR LATER
    cv::Mat decodingMatrix(4, 4, CV_32S);
    for (unsigned int i = 0; i < 16; i++) {
        decodingMatrix.at<int>(i / 4, i % 4) = qRound(pow(2.0, (double)i));
    }

    int validCodeCounter = 0;
    for (unsigned int n = 0; n < squares.size(); n++) {
        // DERIVE SQUARE COORDINATES TO IMAGE PIXEL COORDINATES BASED ON CURRENT SQUARE
        cv::vector<cv::Point2f> inPoints;
        for (int c = 0; c < 4; c++) {
            inPoints.push_back(squares[n][c]);
        }
        cv::Mat localTransform = cv::getPerspectiveTransform(sqPoints, inPoints);

        // MAP THE CODE BIT COORDINATES TO IMAGE PIXEL COORDINATES
        cv::Mat codeMatrix(4, 4, CV_32S);
        cv::transform(gdPoints, kgPoints, localTransform);
        for (unsigned int i = 0; i < kgPoints.size(); i++) {
            // ACCUMULATE THE SUM OF PIXELS WITHIN A SMALL WINDOW ABOUT CURRENT PIXEL
            int row = qRound(kgPoints[i].y / kgPoints[i].z);
            int col = qRound(kgPoints[i].x / kgPoints[i].z);

            // MAKE SURE THE CURRENT COORDINATE IS WITHIN THE BOUNDS OF THE IMAGE
            if (row > 0 && row < image.rows - 1) {
                if (col > 0 && col < image.cols - 1) {
                    int sum = 0;
                    for (int r = row - 1; r < row + 2; r++) {
                        for (int c = col - 1; c < col + 2; c++) {
                            sum += (image.at<unsigned char>(r, c) > 128);
                        }
                    }
                    // COPY THE CURRENT BIT TO THE CODE MATRIX
                    if (sum > 4) {
                        codeMatrix.at<int>(i / 4, 3 - i % 4) = 1;
                    } else {
                        codeMatrix.at<int>(i / 4, 3 - i % 4) = 0;
                    }
                }
            }
        }

        // FLIP THE CODE MATRIX LEFT-RIGHT IN CASE WE ARE
        // LOOKING AT THE TARGET FROM BEHIND WITH BACK LIGHTING
        if (flipCalTagsFlag) {
            cv::flip(codeMatrix, codeMatrix, 1);

            cv::Point2f point;
            unsigned int code = decodingMatrix.dot(codeMatrix);
            if (checkBitCode(code, &point) == false) {
                cv::Mat matrix = codeMatrix;
                cv::transpose(matrix, matrix);
                cv::flip(matrix, matrix, 1);
                code = decodingMatrix.dot(matrix);

                if (checkBitCode(code, &point) == false) {
                    cv::transpose(matrix, matrix);
                    cv::flip(matrix, matrix, 1);
                    code = decodingMatrix.dot(matrix);

                    if (checkBitCode(code, &point) == false) {
                        cv::transpose(matrix, matrix);
                        cv::flip(matrix, matrix, 1);
                        code = decodingMatrix.dot(matrix);

                        if (checkBitCode(code, &point) == false) {
                            cv::vector<cv::Point2f> square;
                            square.push_back(cv::Point2f(NAN, NAN));
                            square.push_back(cv::Point2f(NAN, NAN));
                            square.push_back(cv::Point2f(NAN, NAN));
                            square.push_back(cv::Point2f(NAN, NAN));
                            outputSquares.push_back(square);
                        } else {
                            validCodeCounter++;
                            cv::vector<cv::Point2f> square;
                            square.push_back(sqPoints[3] + point);
                            square.push_back(sqPoints[0] + point);
                            square.push_back(sqPoints[1] + point);
                            square.push_back(sqPoints[2] + point);
                            outputSquares.push_back(square);
                        }
                    } else {
                        validCodeCounter++;
                        cv::vector<cv::Point2f> square;
                        square.push_back(sqPoints[0] + point);
                        square.push_back(sqPoints[1] + point);
                        square.push_back(sqPoints[2] + point);
                        square.push_back(sqPoints[3] + point);
                        outputSquares.push_back(square);
                    }
                } else {
                    validCodeCounter++;
                    cv::vector<cv::Point2f> square;
                    square.push_back(sqPoints[1] + point);
                    square.push_back(sqPoints[2] + point);
                    square.push_back(sqPoints[3] + point);
                    square.push_back(sqPoints[0] + point);
                    outputSquares.push_back(square);
                }
            } else {
                cv::vector<cv::Point2f> square;
                square.push_back(sqPoints[2] + point);
                square.push_back(sqPoints[3] + point);
                square.push_back(sqPoints[0] + point);
                square.push_back(sqPoints[1] + point);
                outputSquares.push_back(square);
            }
        } else {
            cv::Point2f point;
            unsigned int code = decodingMatrix.dot(codeMatrix);
            if (checkBitCode(code, &point) == false) {
                cv::Mat matrix = codeMatrix;
                cv::transpose(matrix, matrix);
                cv::flip(matrix, matrix, 1);
                code = decodingMatrix.dot(matrix);

                if (checkBitCode(code, &point) == false) {
                    cv::transpose(matrix, matrix);
                    cv::flip(matrix, matrix, 1);
                    code = decodingMatrix.dot(matrix);

                    if (checkBitCode(code, &point) == false) {
                        cv::transpose(matrix, matrix);
                        cv::flip(matrix, matrix, 1);
                        code = decodingMatrix.dot(matrix);

                        if (checkBitCode(code, &point) == false) {
                            cv::vector<cv::Point2f> square;
                            square.push_back(cv::Point2f(NAN, NAN));
                            square.push_back(cv::Point2f(NAN, NAN));
                            square.push_back(cv::Point2f(NAN, NAN));
                            square.push_back(cv::Point2f(NAN, NAN));
                            outputSquares.push_back(square);
                        } else {
                            validCodeCounter++;
                            cv::vector<cv::Point2f> square;
                            square.push_back(sqPoints[2] + point);
                            square.push_back(sqPoints[1] + point);
                            square.push_back(sqPoints[0] + point);
                            square.push_back(sqPoints[3] + point);
                            outputSquares.push_back(square);
                        }
                    } else {
                        validCodeCounter++;
                        cv::vector<cv::Point2f> square;
                        square.push_back(sqPoints[3] + point);
                        square.push_back(sqPoints[2] + point);
                        square.push_back(sqPoints[1] + point);
                        square.push_back(sqPoints[0] + point);
                        outputSquares.push_back(square);
                    }
                } else {
                    validCodeCounter++;
                    cv::vector<cv::Point2f> square;
                    square.push_back(sqPoints[0] + point);
                    square.push_back(sqPoints[3] + point);
                    square.push_back(sqPoints[2] + point);
                    square.push_back(sqPoints[1] + point);
                    outputSquares.push_back(square);
                }
            } else {
                cv::vector<cv::Point2f> square;
                square.push_back(sqPoints[1] + point);
                square.push_back(sqPoints[0] + point);
                square.push_back(sqPoints[3] + point);
                square.push_back(sqPoints[2] + point);
                outputSquares.push_back(square);
            }
        }
    }
    return (outputSquares);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
bool LAUCalTagGLWidget::checkBitCode(int code, cv::Point2f *pt)
{
    // DEFINE LOOK-UP TABLE
    static const int realBitCodes[20][14] = {
        {19853,	19918,	20121,	20186,	20373,	20438,	20497,	20562,	20692,	20765,	20830,	20891,	20952,	21001},
        {18674,	18747,	18808,	18877,	18942,	18991,	19052,	19113,	19235,	19296,	19365,	19650,	19723,	19784},
        {17633,	17704,	17771,	17838,	17901,	17980,	18047,	18106,	18224,	18291,	18358,	18421,	18487,	18609},
        {16530,	16593,	16664,	16731,	16798,	16861,	16908,	16975,	17034,	17286,	17349,	17444,	17511,	17570},
        {15514,	15577,	15632,	15699,	15766,	15829,	15943,	16002,	16065,	16136,	16203,	16270,	16333,	16471},
        {14380,	14447,	14506,	14569,	14624,	14691,	14758,	14821,	14967,	15089,	15160,	15294,	15357,	15455},
        {13375,	13436,	13497,	13562,	13680,	13749,	13814,	13863,	13924,	13985,	14050,	14123,	14253,	14318},
        {12303,	12364,	12425,	12490,	12547,	12677,	12742,	12884,	12945,	13010,	13083,	13144,	13213,	13278},
        {11195,	11256,	11289,	11423,	11484,	11541,	11606,	11667,	11777,	11842,	11911,	11972,	12110,	12171},
        {10219,	10281,	10346,	10415,	10533,	10598,	10659,	10720,	10801,	10866,	10935,	10996,	11069,	11134},
        {9179,	9274,	9337,	9404,	9471,	9526,	9589,	9648,	9715,	9825,	9892,	9959,	10030,	10093},
        {8001,	8068,	8135,	8265,	8399,	8454,	8517,	8643,	8722,	8852,	8919,	8990,	9053,	9112},
        {7092,	7159,	7190,	7253,	7312,	7379,	7450,	7580,	7647,	7694,	7757,	7816,	7883,	7938},
        {6116,	6245,	6304,	6371,	6442,	6505,	6572,	6639,	6718,	6781,	6840,	6907,	6962,	7025},
        {5173,	5238,	5299,	5360,	5433,	5498,	5567,	5628,	5677,	5742,	5803,	5864,	5986,	6055},
        {4166,	4227,	4361,	4426,	4495,	4556,	4637,	4702,	4763,	4824,	4881,	4946,	5015,	5076},
        {3152,	3221,	3286,	3359,	3420,	3481,	3546,	3595,	3656,	3725,	3847,	3908,	3969,	4034},
        {2278,	2351,	2412,	2473,	2538,	2619,	2680,	2749,	2814,	2871,	2932,	2993,	3058,	3091},
        {1340,	1407,	1466,	1529,	1576,	1643,	1710,	1773,	1828,	1895,	1954,	2017,	2083,	2213},
        {197,   335,	394,	457,	603,	670,	733,	788,	855,	914,	977,	1139,	1206,	1269}
    };

    for (int i = 0; i < 20; i++) {
        for (int j = 13; j >= 0; j--) {
            if (code == realBitCodes[i][j]) {
                *pt = cv::Point2f((float)(20 - i) - 10.0f, (float)(13 - j) - 6.0f);
                return (true);
            } else if (code > realBitCodes[i][j]) {
                return (false);
            }
        }
    }
    return (false);
}


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
cv::vector<cv::Point2f> LAUCalTagGLWidget::removeOutlierPoints(cv::vector<cv::Point2f> fmPoints, cv::vector<cv::Point2f> toPoints)
{
    // MAKE SURE WE HAVE ENOUGH POINTS TO MAKE THIS WORTH WHILE
    if (fmPoints.size() < 20) {
        return (toPoints);
    }

    cv::Mat transform;
    cv::vector<cv::Point2f> nwPoints = fmPoints;
    unsigned int numIterations = fmPoints.size() * 0.05;
    for (unsigned int iter = 0; iter < numIterations; iter++) {
        // GET THE BEST MAPPING BASED ON THE CURRENT SET OF POINTS
        transform = findBestLinearMapping(fmPoints, toPoints);

        // SEARCH THROUGH LIST LOOKING FOR WORST CASE POINT AND DELETE IT FROM THE LIST
        double optError = 0.0;
        unsigned int optIndex = 0;
        for (unsigned int n = 0; n < fmPoints.size(); n++) {
            double z =  fmPoints[n].x * transform.at<double>(2, 0) + fmPoints[n].y * transform.at<double>(2, 1) + transform.at<double>(2, 2);
            double x = (fmPoints[n].x * transform.at<double>(0, 0) + fmPoints[n].y * transform.at<double>(0, 1) + transform.at<double>(0, 2)) / z;
            double y = (fmPoints[n].x * transform.at<double>(1, 0) + fmPoints[n].y * transform.at<double>(1, 1) + transform.at<double>(1, 2)) / z;
            double e = (x - toPoints[n].x) * (x - toPoints[n].x) + (y - toPoints[n].y) * (y - toPoints[n].y);
            if (e > optError) {
                optIndex = n;
                optError = e;
            }
        }
        // DELETE THE WORST CASE POINT PAIR FROM OUR LISTS
        fmPoints.erase(fmPoints.begin() + optIndex);
        toPoints.erase(toPoints.begin() + optIndex);
    }

    // AT THIS POINT, WE SHOULD HAVE A GOOD FIT, ABSENT ANY OUTLIERS
    // SO LETS REASSIGN THE XY COORDINATES OF OUR TO-POINTS BASED ON THE
    // CURRENT TRANSFORM
    for (unsigned int n = 0; n < nwPoints.size(); n++) {
        double z =  nwPoints[n].x * transform.at<double>(2, 0) + nwPoints[n].y * transform.at<double>(2, 1) + transform.at<double>(2, 2);
        double x = (nwPoints[n].x * transform.at<double>(0, 0) + nwPoints[n].y * transform.at<double>(0, 1) + transform.at<double>(0, 2)) / z;
        double y = (nwPoints[n].x * transform.at<double>(1, 0) + nwPoints[n].y * transform.at<double>(1, 1) + transform.at<double>(1, 2)) / z;

        nwPoints[n] = cv::Point2f((float)qRound(x), (float)qRound(y));
    }

    return (nwPoints);
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
cv::Mat LAUCalTagGLWidget::findBestLinearMapping(cv::vector<cv::Point2f> fmPoints, cv::vector<cv::Point2f> toPoints)
{
    cv::Mat transformMatrix(3, 3, CV_64F);

    // MAKE SURE WE FOUND ENOUGH CLUSTERS TO DO A DECENT INTERPOLATION
    int N = (int)fmPoints.size();

    // ALLOCATE SPACE FOR DATA VECTORS
    cv::vector<double> cVec(N);
    cv::vector<double> rVec(N);
    cv::vector<double> xVec(N);
    cv::vector<double> yVec(N);

    // COPY DATA FROM CLUSTER LIST INTO DATA VECTORS
    for (int n = 0; n < N; n++) {
        cVec[n] = fmPoints[n].x;
        rVec[n] = fmPoints[n].y;

        xVec[n] = toPoints[n].x;
        yVec[n] = toPoints[n].y;
    }

    // CREATE MATRIX TO HOLD THE 4TH ORDER TRANSFORM
    cv::Mat A(N, 3, CV_64F);
    cv::Mat B(N, 2, CV_64F);

    for (int r = 0; r < N; r++) {
        // POPULATE THE ODD NUMBERED ROWS
        A.at<double>(r, 0) = cVec[r];
        A.at<double>(r, 1) = rVec[r];
        A.at<double>(r, 2) = 1.0;

        B.at<double>(r, 0) = xVec[r];
        B.at<double>(r, 1) = yVec[r];
    }
    cv::Mat sVec = (A.t() * A).inv() * A.t() * B;

    transformMatrix.at<double>(0, 0) = sVec.at<double>(0, 0);
    transformMatrix.at<double>(0, 1) = sVec.at<double>(1, 0);
    transformMatrix.at<double>(0, 2) = sVec.at<double>(2, 0);
    transformMatrix.at<double>(1, 0) = sVec.at<double>(0, 1);
    transformMatrix.at<double>(1, 1) = sVec.at<double>(1, 1);
    transformMatrix.at<double>(1, 2) = sVec.at<double>(2, 1);
    transformMatrix.at<double>(2, 0) = 0.0;
    transformMatrix.at<double>(2, 1) = 0.0;
    transformMatrix.at<double>(2, 2) = 1.0;

    return (transformMatrix);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
cv::Mat LAUCalTagGLWidget::findBestQuadraticMapping(cv::vector<cv::Point2f> fmPoints, cv::vector<cv::Point2f> toPoints, int order)
{
    cv::Mat lVec(30, 1, CV_64F);
    lVec.setTo(0.0);

    int numPoints = (int)fmPoints.size();

    // ALLOCATE SPACE FOR DATA VECTORS
    cv::vector<double> rVec(numPoints);
    cv::vector<double> cVec(numPoints);
    cv::vector<double> xVec(numPoints);
    cv::vector<double> yVec(numPoints);

    // COPY DATA FROM CLUSTER LIST INTO DATA VECTORS
    for (int n = 0; n < numPoints; n++) {
        cVec[n] = (fmPoints[n].x - videoTexture->width() / 2) / 50.0;
        rVec[n] = (fmPoints[n].y - videoTexture->height() / 2) / 50.0;
        xVec[n] = toPoints[n].x;
        yVec[n] = toPoints[n].y;
    }

    if (order == 4) {
        // MAKE SURE WE FOUND ENOUGH CLUSTERS TO DO A DECENT INTERPOLATION
        if (fmPoints.size() >= 15) {
            // CREATE MATRIX TO HOLD THE 4TH ORDER TRANSFORM
            cv::Mat A(2 * numPoints, 30, CV_64F);
            A.setTo(0.0);
            cv::Mat B(2 * numPoints,  1, CV_64F);

            for (int r = 0; r < numPoints; r++) {
                // POPULATE THE ODD NUMBERED ROWS
                A.at<double>(2 * r + 0, 0)  = cVec[r] * cVec[r] * cVec[r] * cVec[r];
                A.at<double>(2 * r + 0, 1)  = cVec[r] * cVec[r] * cVec[r] * rVec[r];
                A.at<double>(2 * r + 0, 2)  = cVec[r] * cVec[r] * rVec[r] * rVec[r];
                A.at<double>(2 * r + 0, 3)  = cVec[r] * rVec[r] * rVec[r] * rVec[r];
                A.at<double>(2 * r + 0, 4)  = rVec[r] * rVec[r] * rVec[r] * rVec[r];
                A.at<double>(2 * r + 0, 5)  = cVec[r] * cVec[r] * cVec[r];
                A.at<double>(2 * r + 0, 6)  = cVec[r] * cVec[r] * rVec[r];
                A.at<double>(2 * r + 0, 7)  = cVec[r] * rVec[r] * rVec[r];
                A.at<double>(2 * r + 0, 8)  = rVec[r] * rVec[r] * rVec[r];
                A.at<double>(2 * r + 0, 9)  = cVec[r] * cVec[r];
                A.at<double>(2 * r + 0, 10) = cVec[r] * rVec[r];
                A.at<double>(2 * r + 0, 11) = rVec[r] * rVec[r];
                A.at<double>(2 * r + 0, 12) = cVec[r];
                A.at<double>(2 * r + 0, 13) = rVec[r];
                A.at<double>(2 * r + 0, 14) = 1.0;

                A.at<double>(2 * r + 1, 15) = cVec[r] * cVec[r] * cVec[r] * cVec[r];
                A.at<double>(2 * r + 1, 16) = cVec[r] * cVec[r] * cVec[r] * rVec[r];
                A.at<double>(2 * r + 1, 17) = cVec[r] * cVec[r] * rVec[r] * rVec[r];
                A.at<double>(2 * r + 1, 18) = cVec[r] * rVec[r] * rVec[r] * rVec[r];
                A.at<double>(2 * r + 1, 19) = rVec[r] * rVec[r] * rVec[r] * rVec[r];
                A.at<double>(2 * r + 1, 20) = cVec[r] * cVec[r] * cVec[r];
                A.at<double>(2 * r + 1, 21) = cVec[r] * cVec[r] * rVec[r];
                A.at<double>(2 * r + 1, 22) = cVec[r] * rVec[r] * rVec[r];
                A.at<double>(2 * r + 1, 23) = rVec[r] * rVec[r] * rVec[r];
                A.at<double>(2 * r + 1, 24) = cVec[r] * cVec[r];
                A.at<double>(2 * r + 1, 25) = cVec[r] * rVec[r];
                A.at<double>(2 * r + 1, 26) = rVec[r] * rVec[r];
                A.at<double>(2 * r + 1, 27) = cVec[r];
                A.at<double>(2 * r + 1, 28) = rVec[r];
                A.at<double>(2 * r + 1, 29) = 1.0;

                B.at<double>(2 * r + 0) = xVec[r];
                B.at<double>(2 * r + 1) = yVec[r];
            }
            lVec = (A.t() * A).inv() * A.t() * B;
        }
    } else if (order == 3) {
        // MAKE SURE WE FOUND ENOUGH CLUSTERS TO DO A DECENT INTERPOLATION
        if (fmPoints.size() >= 10) {
            // CREATE MATRIX TO HOLD THE 4TH ORDER TRANSFORM
            cv::Mat A(2 * numPoints, 20, CV_64F);
            A.setTo(0.0);
            cv::Mat B(2 * numPoints,  1, CV_64F);

            for (int r = 0; r < numPoints; r++) {
                // POPULATE THE ODD NUMBERED ROWS
                A.at<double>(2 * r + 0, 0) = cVec[r] * cVec[r] * cVec[r];
                A.at<double>(2 * r + 0, 1) = cVec[r] * cVec[r] * rVec[r];
                A.at<double>(2 * r + 0, 2) = cVec[r] * rVec[r] * rVec[r];
                A.at<double>(2 * r + 0, 3) = rVec[r] * rVec[r] * rVec[r];
                A.at<double>(2 * r + 0, 4) = cVec[r] * cVec[r];
                A.at<double>(2 * r + 0, 5) = cVec[r] * rVec[r];
                A.at<double>(2 * r + 0, 6) = rVec[r] * rVec[r];
                A.at<double>(2 * r + 0, 7) = cVec[r];
                A.at<double>(2 * r + 0, 8) = rVec[r];
                A.at<double>(2 * r + 0, 9) = 1.0;

                A.at<double>(2 * r + 1, 10) = cVec[r] * cVec[r] * cVec[r];
                A.at<double>(2 * r + 1, 11) = cVec[r] * cVec[r] * rVec[r];
                A.at<double>(2 * r + 1, 12) = cVec[r] * rVec[r] * rVec[r];
                A.at<double>(2 * r + 1, 13) = rVec[r] * rVec[r] * rVec[r];
                A.at<double>(2 * r + 1, 14) = cVec[r] * cVec[r];
                A.at<double>(2 * r + 1, 15) = cVec[r] * rVec[r];
                A.at<double>(2 * r + 1, 16) = rVec[r] * rVec[r];
                A.at<double>(2 * r + 1, 17) = cVec[r];
                A.at<double>(2 * r + 1, 18) = rVec[r];
                A.at<double>(2 * r + 1, 19) = 1.0;

                B.at<double>(2 * r + 0) = xVec[r];
                B.at<double>(2 * r + 1) = yVec[r];
            }
            cv::Mat sVec = (A.t() * A).inv() * A.t() * B;

            // COPY SVECTOR OVER TO LVECTOR
            for (int n = 0; n < 10; n++) {
                lVec.at<double>(n + 5) = sVec.at<double>(n + 0);
                lVec.at<double>(n + 20) = sVec.at<double>(n + 10);
            }
        }
    } else if (order == 2) {
        // MAKE SURE WE FOUND ENOUGH CLUSTERS TO DO A DECENT INTERPOLATION
        if (fmPoints.size() >= 6) {
            // CREATE MATRIX TO HOLD THE 4TH ORDER TRANSFORM
            cv::Mat A(2 * numPoints, 12, CV_64F);
            A.setTo(0.0);
            cv::Mat B(2 * numPoints,  1, CV_64F);

            for (int r = 0; r < numPoints; r++) {
                // POPULATE THE ODD NUMBERED ROWS
                A.at<double>(2 * r + 0, 0) = cVec[r] * cVec[r];
                A.at<double>(2 * r + 0, 1) = cVec[r] * rVec[r];
                A.at<double>(2 * r + 0, 2) = rVec[r] * rVec[r];
                A.at<double>(2 * r + 0, 3) = cVec[r];
                A.at<double>(2 * r + 0, 4) = rVec[r];
                A.at<double>(2 * r + 0, 5) = 1.0;

                A.at<double>(2 * r + 1, 6)  = cVec[r] * cVec[r];
                A.at<double>(2 * r + 1, 7)  = cVec[r] * rVec[r];
                A.at<double>(2 * r + 1, 8)  = rVec[r] * rVec[r];
                A.at<double>(2 * r + 1, 9)  = cVec[r];
                A.at<double>(2 * r + 1, 10) = rVec[r];
                A.at<double>(2 * r + 1, 11) = 1.0;

                B.at<double>(2 * r + 0) = xVec[r];
                B.at<double>(2 * r + 1) = yVec[r];
            }
            cv::Mat sVec = (A.t() * A).inv() * A.t() * B;

            // COPY SVECTOR OVER TO LVECTOR
            for (int n = 0; n < 6; n++) {
                lVec.at<double>(n + 9) = sVec.at<double>(n + 0);
                lVec.at<double>(n + 24) = sVec.at<double>(n + 6);
            }
        }
    } else if (order == 1) {
        // MAKE SURE WE FOUND ENOUGH CLUSTERS TO DO A DECENT INTERPOLATION
        if (fmPoints.size() >= 3) {
            // CREATE MATRIX TO HOLD THE 4TH ORDER TRANSFORM
            cv::Mat A(2 * numPoints, 6, CV_64F);
            A.setTo(0.0);
            cv::Mat B(2 * numPoints, 1, CV_64F);

            for (int r = 0; r < numPoints; r++) {
                // POPULATE THE ODD NUMBERED ROWS
                A.at<double>(2 * r + 0, 0) = cVec[r];
                A.at<double>(2 * r + 0, 1) = rVec[r];
                A.at<double>(2 * r + 0, 2) = 1.0;

                A.at<double>(2 * r + 1, 3) = cVec[r];
                A.at<double>(2 * r + 1, 4) = rVec[r];
                A.at<double>(2 * r + 1, 5) = 1.0;

                B.at<double>(2 * r + 0) = xVec[r];
                B.at<double>(2 * r + 1) = yVec[r];
            }
            cv::Mat sVec = (A.t() * A).inv() * A.t() * B;

            // COPY SVECTOR OVER TO LVECTOR
            for (int n = 0; n < 3; n++) {
                lVec.at<double>(n + 12) = sVec.at<double>(n + 0);
                lVec.at<double>(n + 27) = sVec.at<double>(n + 3);
            }
        }
    }
    return (lVec);
}
