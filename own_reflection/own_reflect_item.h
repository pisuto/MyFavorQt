#ifndef OWN_REFLECT_ITEM_H
#define OWN_REFLECT_ITEM_H

#include "reflection/ref_define.h"

/* 定义多个用于反射的类 */

struct osize {
    size_t width;
    size_t height;

    REFLECT(osize)
};

struct ogrid {
    size_t row;
    size_t col;

    REFLECT(ogrid)
};

struct oimage {
    std::string name;
    std::string path;

    REFLECT(oimage)
};

struct ofont {
    std::string key; /* 标明widget的名字，其实是按照顺序来 */
    int extent;
    int weight;
    std::string name;
    std::string style;

    REFLECT(ofont)
};

struct orgba {
    int red;
    int green;
    int blue;
    int alpha;

    REFLECT(orgba)
};

/* category */
struct ocategory {
    std::vector<oimage> categories;

    REFLECT(ocategory)
};

/* button */
struct obutton {
    int id;
    osize size;
    osize icon;
    ofont font;
    std::vector<orgba> colors; /* 对应不同的状态 */

    REFLECT(obutton)
};

/* button group */
struct obtngrp {
    int id;        /* 不在动画中的id */
    size_t offset; /* 不在动画中的偏移，一般为子button长度的叠加 */
    std::vector<obutton> btns;

    REFLECT(obtngrp)
};


/* screen */
struct oresolution {
    osize size;
    osize image;

    REFLECT(oresolution)
};

struct oscreen {
    size_t index; /* 选中的分辨率 */
    ogrid grid;   /* 分布情况 */
    std::vector<oresolution> resogrp;

    REFLECT(oscreen)
};

/* component */
struct ocomponent {
    std::vector<ofont> fonts;

    REFLECT(ocomponent)
};

struct osetting {
    obtngrp btngrp;

    REFLECT(osetting)
};

/* configuration */
struct oconfig {
    bool trayed; /* 最小化 */
    ocategory category;
    oscreen screen;
    /* 组件 */
    obtngrp btngrp;
    ocomponent element;
    ocomponent pagebar;
    osetting setting;

    REFLECT(oconfig)
};




#endif // OWN_REFLECT_ITEM_H
