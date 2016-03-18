/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef Sk4fGradientBase_DEFINED
#define Sk4fGradientBase_DEFINED

#include "Sk4fGradientPriv.h"
#include "SkColor.h"
#include "SkGradientShaderPriv.h"
#include "SkMatrix.h"
#include "SkNx.h"
#include "SkPM4f.h"
#include "SkShader.h"
#include "SkTArray.h"

class SkGradientShaderBase::
GradientShaderBase4fContext : public SkShader::Context {
public:
    GradientShaderBase4fContext(const SkGradientShaderBase&,
                                const ContextRec&);

    uint32_t getFlags() const override { return fFlags; }

    void shadeSpan(int x, int y, SkPMColor dst[], int count) override;
    void shadeSpan4f(int x, int y, SkPM4f dst[], int count) override;

protected:
    struct Interval {
        Interval(SkPMColor c0, SkScalar p0,
                 SkPMColor c1, SkScalar p1,
                 const Sk4f& componentScale);

        bool isZeroRamp() const { return fZeroRamp; }

        SkPM4f   fC0, fDc;
        SkScalar fP0, fP1;
        bool     fZeroRamp;
    };

    virtual void mapTs(int x, int y, SkScalar ts[], int count) const = 0;

    void buildIntervals(const SkGradientShaderBase&, const ContextRec&, bool reverse);

    SkSTArray<8, Interval, true> fIntervals;
    SkMatrix                     fDstToPos;
    SkMatrix::MapXYProc          fDstToPosProc;
    uint8_t                      fDstToPosClass;
    uint8_t                      fFlags;
    bool                         fDither;
    bool                         fColorsArePremul;

private:
    using INHERITED = SkShader::Context;

    void addMirrorIntervals(const SkGradientShaderBase&,
                            const Sk4f& componentScale, bool reverse);

    template<typename DstType, SkShader::TileMode tileMode>
    class TSampler;

    template <typename DstType, ApplyPremul>
    void shadePremulSpan(int x, int y, DstType[], int count) const;

    template <typename DstType, ApplyPremul, SkShader::TileMode tileMode>
    void shadeSpanInternal(int x, int y, DstType[], int count) const;
};

#endif // Sk4fGradientBase_DEFINED
