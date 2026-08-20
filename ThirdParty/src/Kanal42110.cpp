/*******************************************************************************************************************
Copyright (c) 2023 Cycling '74

The code that Max generates automatically and that end users are capable of
exporting and using, and any associated documentation files (the “Software”)
is a work of authorship for which Cycling '74 is the author and owner for
copyright purposes.

This Software is dual-licensed either under the terms of the Cycling '74
License for Max-Generated Code for Export, or alternatively under the terms
of the General Public License (GPL) Version 3. You may use the Software
according to either of these licenses as it is most appropriate for your
project on a case-by-case basis (proprietary or not).

A) Cycling '74 License for Max-Generated Code for Export

A license is hereby granted, free of charge, to any person obtaining a copy
of the Software (“Licensee”) to use, copy, modify, merge, publish, and
distribute copies of the Software, and to permit persons to whom the Software
is furnished to do so, subject to the following conditions:

The Software is licensed to Licensee for all uses that do not include the sale,
sublicensing, or commercial distribution of software that incorporates this
source code. This means that the Licensee is free to use this software for
educational, research, and prototyping purposes, to create musical or other
creative works with software that incorporates this source code, or any other
use that does not constitute selling software that makes use of this source
code. Commercial distribution also includes the packaging of free software with
other paid software, hardware, or software-provided commercial services.

For entities with UNDER $200k in annual revenue or funding, a license is hereby
granted, free of charge, for the sale, sublicensing, or commercial distribution
of software that incorporates this source code, for as long as the entity's
annual revenue remains below $200k annual revenue or funding.

For entities with OVER $200k in annual revenue or funding interested in the
sale, sublicensing, or commercial distribution of software that incorporates
this source code, please send inquiries to licensing@cycling74.com.

The above copyright notice and this license shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Please see
https://support.cycling74.com/hc/en-us/articles/10730637742483-RNBO-Export-Licensing-FAQ
for additional information

B) General Public License Version 3 (GPLv3)
Details of the GPLv3 license can be found at: https://www.gnu.org/licenses/gpl-3.0.html
*******************************************************************************************************************/

#include "RNBO_Common.h"
#include "RNBO_AudioSignal.h"

namespace RNBO {


#define trunc(x) ((Int)(x))

#if defined(__GNUC__) || defined(__clang__)
    #define RNBO_RESTRICT __restrict__
#elif defined(_MSC_VER)
    #define RNBO_RESTRICT __restrict
#endif

#define FIXEDSIZEARRAYINIT(...) { }

class Kanal42110 : public PatcherInterfaceImpl {
public:

class RNBOSubpatcher_05 : public PatcherInterfaceImpl {
    
    friend class Kanal42110;
    
    public:
    
    RNBOSubpatcher_05()
    {
    }
    
    ~RNBOSubpatcher_05()
    {
    }
    
    virtual Kanal42110* getPatcher() const {
        return static_cast<Kanal42110 *>(_parentPatcher);
    }
    
    Kanal42110* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
        getEngine()->flushClockEvents(this, -1621164530, false);
        getEngine()->flushClockEvents(this, -62043057, false);
        getEngine()->flushClockEvents(this, -1494586265, false);
    }
    
    inline number safesqrt(number num) {
        return (num > 0.0 ? rnbo_sqrt(num) : 0.0);
    }
    
    Index vectorsize() const {
        return this->vs;
    }
    
    SampleIndex currentsampletime() {
        return this->audioProcessSampleCount + this->sampleOffsetIntoNextAudioBuffer;
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    Index getNumMidiInputPorts() const {
        return 0;
    }
    
    void processMidiEvent(MillisecondTime , int , ConstByteArray , Index ) {}
    
    Index getNumMidiOutputPorts() const {
        return 0;
    }
    
    void process(
        const SampleValue * const* inputs,
        Index numInputs,
        SampleValue * const* outputs,
        Index numOutputs,
        Index n
    ) {
        this->vs = n;
        this->updateTime(this->getEngine()->getCurrentTime());
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        SampleValue * out2 = (numOutputs >= 2 && outputs[1] ? outputs[1] : this->dummyBuffer);
        const SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        const SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        this->linetilde_01_perform(this->signals[0], n);
        this->linetilde_02_perform(this->signals[1], n);
        this->numbertilde_01_perform(this->signals[1], this->dummyBuffer, n);
    
        this->filtercoeff_01_perform(
            this->filtercoeff_01_frequency,
            this->signals[0],
            this->signals[1],
            this->signals[2],
            this->signals[3],
            this->signals[4],
            this->signals[5],
            this->signals[6],
            n
        );
    
        this->biquad_tilde_02_perform(
            in2,
            this->signals[2],
            this->signals[3],
            this->signals[4],
            this->signals[5],
            this->signals[6],
            out2,
            n
        );
    
        this->biquad_tilde_01_perform(
            in1,
            this->signals[2],
            this->signals[3],
            this->signals[4],
            this->signals[5],
            this->signals[6],
            out1,
            n
        );
    
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 7; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
            this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
            this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
            this->didAllocateSignals = true;
        }
    
        const bool sampleRateChanged = sampleRate != this->sr;
        const bool maxvsChanged = maxBlockSize != this->maxvs;
        const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;
    
        if (sampleRateChanged || maxvsChanged) {
            this->vs = maxBlockSize;
            this->maxvs = maxBlockSize;
            this->sr = sampleRate;
            this->invsr = 1 / sampleRate;
        }
    
        this->numbertilde_01_dspsetup(forceDSPSetup);
        this->filtercoeff_01_dspsetup(forceDSPSetup);
        this->biquad_tilde_02_dspsetup(forceDSPSetup);
        this->biquad_tilde_01_dspsetup(forceDSPSetup);
    
        if (sampleRateChanged)
            this->onSampleRateChanged(sampleRate);
    }
    
    void setProbingTarget(MessageTag id) {
        switch (id) {
        default:
            {
            this->setProbingIndex(-1);
            break;
            }
        }
    }
    
    void setProbingIndex(ProbingIndex ) {}
    
    Index getProbingChannels(MessageTag outletId) const {
        RNBO_UNUSED(outletId);
        return 0;
    }
    
    Index getIsMuted()  {
        return this->isMuted;
    }
    
    void setIsMuted(Index v)  {
        this->isMuted = v;
    }
    
    void onSampleRateChanged(double ) {}
    
    Index getPatcherSerial() const {
        return 0;
    }
    
    void getState(PatcherStateInterface& ) {}
    
    void setState() {}
    
    void getPreset(PatcherStateInterface& ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processParameterBangEvent(ParameterIndex index, MillisecondTime time) {
        this->setParameterValue(index, this->getParameterValue(index), time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
        default:
            {
            return 0;
            }
        }
    }
    
    ParameterIndex getNumSignalInParameters() const {
        return 0;
    }
    
    ParameterIndex getNumSignalOutParameters() const {
        return 0;
    }
    
    ParameterIndex getNumParameters() const {
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            {
            return "bogus";
            }
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            {
            return "bogus";
            }
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
    }
    
    ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
        if (steps == 1) {
            if (normalizedValue > 0) {
                normalizedValue = 1.;
            }
        } else {
            ParameterValue oneStep = (number)1. / (steps - 1);
            ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
            normalizedValue = numberOfSteps * oneStep;
        }
    
        return normalizedValue;
    }
    
    ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
    }
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case -1621164530:
            {
            this->linetilde_01_target_bang();
            break;
            }
        case -62043057:
            {
            this->linetilde_02_target_bang();
            break;
            }
        case -1494586265:
            {
            this->numbertilde_01_value_set(value);
            break;
            }
        }
    }
    
    void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}
    
    void processOutletEvent(
        EngineLink* sender,
        OutletIndex index,
        ParameterValue value,
        MillisecondTime time
    ) {
        this->updateTime(time);
        this->processOutletAtCurrentTime(sender, index, value);
    }
    
    void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("valin"):
            {
            if (TAG("Bassshelf/number_obj-3") == objectId)
                this->numberobj_01_valin_set(payload);
    
            if (TAG("Bassshelf/number_obj-4") == objectId)
                this->numberobj_02_valin_set(payload);
    
            if (TAG("Bassshelf/number_obj-8") == objectId)
                this->numberobj_03_valin_set(payload);
    
            break;
            }
        case TAG("format"):
            {
            if (TAG("Bassshelf/number_obj-3") == objectId)
                this->numberobj_01_format_set(payload);
    
            if (TAG("Bassshelf/number_obj-4") == objectId)
                this->numberobj_02_format_set(payload);
    
            if (TAG("Bassshelf/number_obj-8") == objectId)
                this->numberobj_03_format_set(payload);
    
            break;
            }
        case TAG("listin"):
            {
            if (TAG("Bassshelf/message_obj-33") == objectId)
                this->message_01_listin_number_set(payload);
    
            if (TAG("Bassshelf/message_obj-31") == objectId)
                this->message_02_listin_number_set(payload);
    
            break;
            }
        case TAG("sig"):
            {
            if (TAG("Bassshelf/number~_obj-7") == objectId)
                this->numbertilde_01_sig_number_set(payload);
    
            break;
            }
        case TAG("mode"):
            {
            if (TAG("Bassshelf/number~_obj-7") == objectId)
                this->numbertilde_01_mode_set(payload);
    
            break;
            }
        }
    }
    
    void processListMessage(
        MessageTag tag,
        MessageTag objectId,
        MillisecondTime time,
        const list& payload
    ) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("listin"):
            {
            if (TAG("Bassshelf/message_obj-33") == objectId)
                this->message_01_listin_list_set(payload);
    
            if (TAG("Bassshelf/message_obj-31") == objectId)
                this->message_02_listin_list_set(payload);
    
            break;
            }
        case TAG("sig"):
            {
            if (TAG("Bassshelf/number~_obj-7") == objectId)
                this->numbertilde_01_sig_list_set(payload);
    
            break;
            }
        }
    }
    
    void processBangMessage(MessageTag tag, MessageTag objectId, MillisecondTime time) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("listin"):
            {
            if (TAG("Bassshelf/message_obj-33") == objectId)
                this->message_01_listin_bang_bang();
    
            if (TAG("Bassshelf/message_obj-31") == objectId)
                this->message_02_listin_bang_bang();
    
            break;
            }
        }
    }
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("valout"):
            {
            return "valout";
            }
        case TAG("Bassshelf/number_obj-3"):
            {
            return "Bassshelf/number_obj-3";
            }
        case TAG("setup"):
            {
            return "setup";
            }
        case TAG("Bassshelf/number_obj-4"):
            {
            return "Bassshelf/number_obj-4";
            }
        case TAG("listout"):
            {
            return "listout";
            }
        case TAG("Bassshelf/message_obj-33"):
            {
            return "Bassshelf/message_obj-33";
            }
        case TAG("Bassshelf/number_obj-8"):
            {
            return "Bassshelf/number_obj-8";
            }
        case TAG("monitor"):
            {
            return "monitor";
            }
        case TAG("Bassshelf/number~_obj-7"):
            {
            return "Bassshelf/number~_obj-7";
            }
        case TAG("assign"):
            {
            return "assign";
            }
        case TAG("Bassshelf/message_obj-31"):
            {
            return "Bassshelf/message_obj-31";
            }
        case TAG("valin"):
            {
            return "valin";
            }
        case TAG("format"):
            {
            return "format";
            }
        case TAG("listin"):
            {
            return "listin";
            }
        case TAG("sig"):
            {
            return "sig";
            }
        case TAG("mode"):
            {
            return "mode";
            }
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        default:
            {
            return nullptr;
            }
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 0;
    }
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void processDataViewUpdate(DataRefIndex , MillisecondTime ) {}
    
    void initialize() {
        this->assign_defaults();
        this->setState();
    }
    
    protected:
    
    void eventinlet_01_out1_bang_bang() {
        this->numberobj_02_value_bang();
    }
    
    void eventinlet_01_out1_list_set(const list& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_02_value_set(converted);
        }
    }
    
    void numberobj_01_valin_set(number v) {
        this->numberobj_01_value_set(v);
    }
    
    void numberobj_01_format_set(number v) {
        this->numberobj_01_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
    }
    
    void numberobj_02_valin_set(number v) {
        this->numberobj_02_value_set(v);
    }
    
    void numberobj_02_format_set(number v) {
        this->numberobj_02_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
    }
    
    void message_01_listin_list_set(const list& v) {
        this->message_01_set_set(v);
    }
    
    void message_01_listin_number_set(number v) {
        this->message_01_set_set(v);
    }
    
    void message_01_listin_bang_bang() {
        this->message_01_trigger_bang();
    }
    
    void numberobj_03_valin_set(number v) {
        this->numberobj_03_value_set(v);
    }
    
    void numberobj_03_format_set(number v) {
        this->numberobj_03_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
    }
    
    void numbertilde_01_sig_number_set(number v) {
        this->numbertilde_01_outValue = v;
    }
    
    void numbertilde_01_sig_list_set(const list& v) {
        this->numbertilde_01_outValue = v[0];
    }
    
    void numbertilde_01_mode_set(number v) {
        if (v == 1) {
            this->numbertilde_01_currentMode = 0;
        } else if (v == 2) {
            this->numbertilde_01_currentMode = 1;
        }
    }
    
    void message_02_listin_list_set(const list& v) {
        this->message_02_set_set(v);
    }
    
    void message_02_listin_number_set(number v) {
        this->message_02_set_set(v);
    }
    
    void message_02_listin_bang_bang() {
        this->message_02_trigger_bang();
    }
    
    void linetilde_01_target_bang() {}
    
    void linetilde_02_target_bang() {}
    
    void numbertilde_01_value_set(number ) {}
    
    number msToSamps(MillisecondTime ms, number sampleRate) {
        return ms * sampleRate * 0.001;
    }
    
    MillisecondTime sampsToMs(SampleIndex samps) {
        return samps * (this->invsr * 1000);
    }
    
    Index getMaxBlockSize() const {
        return this->maxvs;
    }
    
    number getSampleRate() const {
        return this->sr;
    }
    
    bool hasFixedVectorSize() const {
        return false;
    }
    
    Index getNumInputChannels() const {
        return 2;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {
        this->numberobj_01_init();
        this->numberobj_02_init();
        this->change_01_init();
        this->message_01_init();
        this->numberobj_03_init();
        this->numbertilde_01_init();
        this->message_02_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {}
    
    void change_01_zero_set(number ) {}
    
    void change_01_nonzero_set(number ) {}
    
    void linetilde_02_time_set(number v) {
        this->linetilde_02_time = v;
    }
    
    void linetilde_02_segments_set(const list& v) {
        this->linetilde_02_segments = jsCreateListCopy(v);
    
        if ((bool)(v->length)) {
            if (v->length == 1 && this->linetilde_02_time == 0) {
                this->linetilde_02_activeRamps->length = 0;
                this->linetilde_02_currentValue = v[0];
            } else {
                auto currentTime = this->currentsampletime();
                number lastRampValue = this->linetilde_02_currentValue;
                number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;
    
                for (Index i = 0; i < this->linetilde_02_activeRamps->length; i += 3) {
                    rampEnd = this->linetilde_02_activeRamps[(Index)(i + 2)];
    
                    if (rampEnd > currentTime) {
                        this->linetilde_02_activeRamps[(Index)(i + 2)] = currentTime;
                        number diff = rampEnd - currentTime;
                        number valueDiff = diff * this->linetilde_02_activeRamps[(Index)(i + 1)];
                        lastRampValue = this->linetilde_02_activeRamps[(Index)i] - valueDiff;
                        this->linetilde_02_activeRamps[(Index)i] = lastRampValue;
                        this->linetilde_02_activeRamps->length = i + 3;
                        rampEnd = currentTime;
                    } else {
                        lastRampValue = this->linetilde_02_activeRamps[(Index)i];
                    }
                }
    
                if (rampEnd < currentTime) {
                    this->linetilde_02_activeRamps->push(lastRampValue);
                    this->linetilde_02_activeRamps->push(0);
                    this->linetilde_02_activeRamps->push(currentTime);
                }
    
                number lastRampEnd = currentTime;
    
                for (Index i = 0; i < v->length; i += 2) {
                    number destinationValue = v[(Index)i];
                    number inc = 0;
                    number rampTimeInSamples;
    
                    if (v->length > i + 1) {
                        rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);
    
                        if ((bool)(this->linetilde_02_keepramp)) {
                            this->linetilde_02_time_set(v[(Index)(i + 1)]);
                        }
                    } else {
                        rampTimeInSamples = this->mstosamps(this->linetilde_02_time);
                    }
    
                    if (rampTimeInSamples <= 0) {
                        rampTimeInSamples = 1;
                    }
    
                    inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                    lastRampEnd += rampTimeInSamples;
                    this->linetilde_02_activeRamps->push(destinationValue);
                    this->linetilde_02_activeRamps->push(inc);
                    this->linetilde_02_activeRamps->push(lastRampEnd);
                    lastRampValue = destinationValue;
                }
            }
        }
    }
    
    void message_01_out_set(const list& v) {
        this->linetilde_02_segments_set(v);
    }
    
    void message_01_trigger_bang() {
        this->message_01_out_set(this->message_01_set);
    }
    
    void select_01_match1_bang() {
        this->message_01_trigger_bang();
    }
    
    void message_02_out_set(const list& v) {
        this->linetilde_02_segments_set(v);
    }
    
    void message_02_trigger_bang() {
        this->message_02_out_set(this->message_02_set);
    }
    
    void select_01_match2_bang() {
        this->message_02_trigger_bang();
    }
    
    void select_01_nomatch_number_set(number ) {}
    
    void select_01_input_number_set(number v) {
        if (v == this->select_01_test1)
            this->select_01_match1_bang();
        else if (v == this->select_01_test2)
            this->select_01_match2_bang();
        else
            this->select_01_nomatch_number_set(v);
    }
    
    void change_01_out_set(number v) {
        this->change_01_out = v;
        this->select_01_input_number_set(v);
    }
    
    void change_01_input_set(number v) {
        this->change_01_input = v;
    
        if (v != this->change_01_prev) {
            number prev = this->change_01_prev;
            this->change_01_prev = v;
    
            if (v == 0) {
                this->change_01_zero_set(1);
            } else if (this->change_01_out == 0) {
                this->change_01_nonzero_set(1);
            }
    
            {
                this->change_01_out_set(v);
            }
        } else {
            this->change_01_prev = v;
        }
    }
    
    void numberobj_03_output_set(number v) {
        this->change_01_input_set(v);
    }
    
    void numberobj_03_value_set(number v) {
        this->numberobj_03_value_setter(v);
        v = this->numberobj_03_value;
        number localvalue = v;
    
        if (this->numberobj_03_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("Bassshelf/number_obj-8"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_03_output_set(localvalue);
    }
    
    void expr_02_out1_set(number v) {
        this->expr_02_out1 = v;
        this->numberobj_03_value_set(this->expr_02_out1);
    }
    
    void expr_02_in1_set(number in1) {
        this->expr_02_in1 = in1;
        this->expr_02_out1_set(this->expr_02_in1 > this->expr_02_in2);//#map:Bassshelf/>_obj-2:1
    }
    
    void linetilde_01_time_set(number v) {
        this->linetilde_01_time = v;
    }
    
    void linetilde_01_segments_set(const list& v) {
        this->linetilde_01_segments = jsCreateListCopy(v);
    
        if ((bool)(v->length)) {
            if (v->length == 1 && this->linetilde_01_time == 0) {
                this->linetilde_01_activeRamps->length = 0;
                this->linetilde_01_currentValue = v[0];
            } else {
                auto currentTime = this->currentsampletime();
                number lastRampValue = this->linetilde_01_currentValue;
                number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;
    
                for (Index i = 0; i < this->linetilde_01_activeRamps->length; i += 3) {
                    rampEnd = this->linetilde_01_activeRamps[(Index)(i + 2)];
    
                    if (rampEnd > currentTime) {
                        this->linetilde_01_activeRamps[(Index)(i + 2)] = currentTime;
                        number diff = rampEnd - currentTime;
                        number valueDiff = diff * this->linetilde_01_activeRamps[(Index)(i + 1)];
                        lastRampValue = this->linetilde_01_activeRamps[(Index)i] - valueDiff;
                        this->linetilde_01_activeRamps[(Index)i] = lastRampValue;
                        this->linetilde_01_activeRamps->length = i + 3;
                        rampEnd = currentTime;
                    } else {
                        lastRampValue = this->linetilde_01_activeRamps[(Index)i];
                    }
                }
    
                if (rampEnd < currentTime) {
                    this->linetilde_01_activeRamps->push(lastRampValue);
                    this->linetilde_01_activeRamps->push(0);
                    this->linetilde_01_activeRamps->push(currentTime);
                }
    
                number lastRampEnd = currentTime;
    
                for (Index i = 0; i < v->length; i += 2) {
                    number destinationValue = v[(Index)i];
                    number inc = 0;
                    number rampTimeInSamples;
    
                    if (v->length > i + 1) {
                        rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);
    
                        if ((bool)(this->linetilde_01_keepramp)) {
                            this->linetilde_01_time_set(v[(Index)(i + 1)]);
                        }
                    } else {
                        rampTimeInSamples = this->mstosamps(this->linetilde_01_time);
                    }
    
                    if (rampTimeInSamples <= 0) {
                        rampTimeInSamples = 1;
                    }
    
                    inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                    lastRampEnd += rampTimeInSamples;
                    this->linetilde_01_activeRamps->push(destinationValue);
                    this->linetilde_01_activeRamps->push(inc);
                    this->linetilde_01_activeRamps->push(lastRampEnd);
                    lastRampValue = destinationValue;
                }
            }
        }
    }
    
    void numberobj_01_output_set(number v) {
        {
            list converted = {v};
            this->linetilde_01_segments_set(converted);
        }
    }
    
    void numberobj_01_value_set(number v) {
        this->numberobj_01_value_setter(v);
        v = this->numberobj_01_value;
        number localvalue = v;
    
        if (this->numberobj_01_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("Bassshelf/number_obj-3"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_01_output_set(localvalue);
    }
    
    void expr_01_out1_set(number v) {
        this->expr_01_out1 = v;
        this->numberobj_01_value_set(this->expr_01_out1);
    }
    
    void expr_01_in1_set(number in1) {
        this->expr_01_in1 = in1;
        this->expr_01_out1_set(rnbo_pow(10, this->expr_01_in1 * 0.05));//#map:Bassshelf/dbtoa_obj-9:1
    }
    
    void numberobj_02_output_set(number v) {
        this->expr_02_in1_set(v);
        this->expr_01_in1_set(v);
    }
    
    void numberobj_02_value_set(number v) {
        this->numberobj_02_value_setter(v);
        v = this->numberobj_02_value;
        number localvalue = v;
    
        if (this->numberobj_02_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("Bassshelf/number_obj-4"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_02_output_set(localvalue);
    }
    
    void eventinlet_01_out1_number_set(number v) {
        this->numberobj_02_value_set(v);
    }
    
    void numberobj_02_value_bang() {
        number v = this->numberobj_02_value;
        number localvalue = v;
    
        if (this->numberobj_02_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("Bassshelf/number_obj-4"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_02_output_set(localvalue);
    }
    
    void message_01_set_set(const list& v) {
        this->message_01_set = jsCreateListCopy(v);
        this->getEngine()->sendListMessage(TAG("listout"), TAG("Bassshelf/message_obj-33"), v, this->_currentTime);
    }
    
    void message_02_set_set(const list& v) {
        this->message_02_set = jsCreateListCopy(v);
        this->getEngine()->sendListMessage(TAG("listout"), TAG("Bassshelf/message_obj-31"), v, this->_currentTime);
    }
    
    void linetilde_01_perform(SampleValue * out, Index n) {
        auto __linetilde_01_time = this->linetilde_01_time;
        auto __linetilde_01_keepramp = this->linetilde_01_keepramp;
        auto __linetilde_01_currentValue = this->linetilde_01_currentValue;
        Index i = 0;
    
        if ((bool)(this->linetilde_01_activeRamps->length)) {
            while ((bool)(this->linetilde_01_activeRamps->length) && i < n) {
                number destinationValue = this->linetilde_01_activeRamps[0];
                number inc = this->linetilde_01_activeRamps[1];
                number rampTimeInSamples = this->linetilde_01_activeRamps[2] - this->audioProcessSampleCount - i;
                number val = __linetilde_01_currentValue;
    
                while (rampTimeInSamples > 0 && i < n) {
                    out[(Index)i] = val;
                    val += inc;
                    i++;
                    rampTimeInSamples--;
                }
    
                if (rampTimeInSamples <= 0) {
                    val = destinationValue;
                    this->linetilde_01_activeRamps->splice(0, 3);
    
                    if ((bool)(!(bool)(this->linetilde_01_activeRamps->length))) {
                        this->getEngine()->scheduleClockEventWithValue(
                            this,
                            -1621164530,
                            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                            0
                        );;
    
                        if ((bool)(!(bool)(__linetilde_01_keepramp))) {
                            __linetilde_01_time = 0;
                        }
                    }
                }
    
                __linetilde_01_currentValue = val;
            }
        }
    
        while (i < n) {
            out[(Index)i] = __linetilde_01_currentValue;
            i++;
        }
    
        this->linetilde_01_currentValue = __linetilde_01_currentValue;
        this->linetilde_01_time = __linetilde_01_time;
    }
    
    void linetilde_02_perform(SampleValue * out, Index n) {
        auto __linetilde_02_time = this->linetilde_02_time;
        auto __linetilde_02_keepramp = this->linetilde_02_keepramp;
        auto __linetilde_02_currentValue = this->linetilde_02_currentValue;
        Index i = 0;
    
        if ((bool)(this->linetilde_02_activeRamps->length)) {
            while ((bool)(this->linetilde_02_activeRamps->length) && i < n) {
                number destinationValue = this->linetilde_02_activeRamps[0];
                number inc = this->linetilde_02_activeRamps[1];
                number rampTimeInSamples = this->linetilde_02_activeRamps[2] - this->audioProcessSampleCount - i;
                number val = __linetilde_02_currentValue;
    
                while (rampTimeInSamples > 0 && i < n) {
                    out[(Index)i] = val;
                    val += inc;
                    i++;
                    rampTimeInSamples--;
                }
    
                if (rampTimeInSamples <= 0) {
                    val = destinationValue;
                    this->linetilde_02_activeRamps->splice(0, 3);
    
                    if ((bool)(!(bool)(this->linetilde_02_activeRamps->length))) {
                        this->getEngine()->scheduleClockEventWithValue(
                            this,
                            -62043057,
                            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                            0
                        );;
    
                        if ((bool)(!(bool)(__linetilde_02_keepramp))) {
                            __linetilde_02_time = 0;
                        }
                    }
                }
    
                __linetilde_02_currentValue = val;
            }
        }
    
        while (i < n) {
            out[(Index)i] = __linetilde_02_currentValue;
            i++;
        }
    
        this->linetilde_02_currentValue = __linetilde_02_currentValue;
        this->linetilde_02_time = __linetilde_02_time;
    }
    
    void numbertilde_01_perform(const SampleValue * input_signal, SampleValue * output, Index n) {
        auto __numbertilde_01_currentIntervalInSamples = this->numbertilde_01_currentIntervalInSamples;
        auto __numbertilde_01_lastValue = this->numbertilde_01_lastValue;
        auto __numbertilde_01_currentInterval = this->numbertilde_01_currentInterval;
        auto __numbertilde_01_rampInSamples = this->numbertilde_01_rampInSamples;
        auto __numbertilde_01_outValue = this->numbertilde_01_outValue;
        auto __numbertilde_01_currentMode = this->numbertilde_01_currentMode;
        number monitorvalue = input_signal[0];
    
        for (Index i = 0; i < n; i++) {
            if (__numbertilde_01_currentMode == 0) {
                output[(Index)i] = this->numbertilde_01_smooth_next(
                    __numbertilde_01_outValue,
                    __numbertilde_01_rampInSamples,
                    __numbertilde_01_rampInSamples
                );
            } else {
                output[(Index)i] = input_signal[(Index)i];
            }
        }
    
        __numbertilde_01_currentInterval -= n;
    
        if (monitorvalue != __numbertilde_01_lastValue && __numbertilde_01_currentInterval <= 0) {
            __numbertilde_01_currentInterval = __numbertilde_01_currentIntervalInSamples;
    
            this->getEngine()->scheduleClockEventWithValue(
                this,
                -1494586265,
                this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                monitorvalue
            );;
    
            __numbertilde_01_lastValue = monitorvalue;
    
            this->getEngine()->sendListMessage(
                TAG("monitor"),
                TAG("Bassshelf/number~_obj-7"),
                {monitorvalue},
                this->_currentTime
            );;
        }
    
        this->numbertilde_01_currentInterval = __numbertilde_01_currentInterval;
        this->numbertilde_01_lastValue = __numbertilde_01_lastValue;
    }
    
    void filtercoeff_01_perform(
        number frequency,
        const Sample * gain,
        const Sample * q,
        SampleValue * a0,
        SampleValue * a1,
        SampleValue * a2,
        SampleValue * b1,
        SampleValue * b2,
        Index n
    ) {
        RNBO_UNUSED(frequency);
        auto __filtercoeff_01_activeResamp = this->filtercoeff_01_activeResamp;
        auto __filtercoeff_01_resamp_counter = this->filtercoeff_01_resamp_counter;
        auto __filtercoeff_01_K_EPSILON = this->filtercoeff_01_K_EPSILON;
    
        for (Index i = 0; i < n; i++) {
            number local_q = q[(Index)i];
            number local_gain = gain[(Index)i];
    
            if (local_gain < 0)
                local_gain = 0;
    
            number local_frequency = 90;
    
            if (local_q < __filtercoeff_01_K_EPSILON)
                local_q = __filtercoeff_01_K_EPSILON;
    
            local_frequency = (local_frequency > this->sr * 0.5 ? this->sr * 0.5 : (local_frequency < 1 ? 1 : local_frequency));
            __filtercoeff_01_resamp_counter--;
    
            if (__filtercoeff_01_resamp_counter <= 0) {
                __filtercoeff_01_resamp_counter = __filtercoeff_01_activeResamp;
                this->filtercoeff_01_updateTerms(local_frequency, local_gain, local_q);
            }
    
            a0[(Index)i] = this->filtercoeff_01_la0;
            a1[(Index)i] = this->filtercoeff_01_la1;
            a2[(Index)i] = this->filtercoeff_01_la2;
            b1[(Index)i] = this->filtercoeff_01_lb1;
            b2[(Index)i] = this->filtercoeff_01_lb2;
        }
    
        this->filtercoeff_01_resamp_counter = __filtercoeff_01_resamp_counter;
    }
    
    void biquad_tilde_02_perform(
        const Sample * x,
        const Sample * a0,
        const Sample * a1,
        const Sample * a2,
        const Sample * b1,
        const Sample * b2,
        SampleValue * out1,
        Index n
    ) {
        auto __biquad_tilde_02_y2 = this->biquad_tilde_02_y2;
        auto __biquad_tilde_02_y1 = this->biquad_tilde_02_y1;
        auto __biquad_tilde_02_x2 = this->biquad_tilde_02_x2;
        auto __biquad_tilde_02_x1 = this->biquad_tilde_02_x1;
        Index i;
    
        for (i = 0; i < n; i++) {
            number tmp = x[(Index)i] * a0[(Index)i] + __biquad_tilde_02_x1 * a1[(Index)i] + __biquad_tilde_02_x2 * a2[(Index)i] - (__biquad_tilde_02_y1 * b1[(Index)i] + __biquad_tilde_02_y2 * b2[(Index)i]);
            __biquad_tilde_02_x2 = __biquad_tilde_02_x1;
            __biquad_tilde_02_x1 = x[(Index)i];
            __biquad_tilde_02_y2 = __biquad_tilde_02_y1;
            __biquad_tilde_02_y1 = tmp;
            out1[(Index)i] = tmp;
        }
    
        this->biquad_tilde_02_x1 = __biquad_tilde_02_x1;
        this->biquad_tilde_02_x2 = __biquad_tilde_02_x2;
        this->biquad_tilde_02_y1 = __biquad_tilde_02_y1;
        this->biquad_tilde_02_y2 = __biquad_tilde_02_y2;
    }
    
    void biquad_tilde_01_perform(
        const Sample * x,
        const Sample * a0,
        const Sample * a1,
        const Sample * a2,
        const Sample * b1,
        const Sample * b2,
        SampleValue * out1,
        Index n
    ) {
        auto __biquad_tilde_01_y2 = this->biquad_tilde_01_y2;
        auto __biquad_tilde_01_y1 = this->biquad_tilde_01_y1;
        auto __biquad_tilde_01_x2 = this->biquad_tilde_01_x2;
        auto __biquad_tilde_01_x1 = this->biquad_tilde_01_x1;
        Index i;
    
        for (i = 0; i < n; i++) {
            number tmp = x[(Index)i] * a0[(Index)i] + __biquad_tilde_01_x1 * a1[(Index)i] + __biquad_tilde_01_x2 * a2[(Index)i] - (__biquad_tilde_01_y1 * b1[(Index)i] + __biquad_tilde_01_y2 * b2[(Index)i]);
            __biquad_tilde_01_x2 = __biquad_tilde_01_x1;
            __biquad_tilde_01_x1 = x[(Index)i];
            __biquad_tilde_01_y2 = __biquad_tilde_01_y1;
            __biquad_tilde_01_y1 = tmp;
            out1[(Index)i] = tmp;
        }
    
        this->biquad_tilde_01_x1 = __biquad_tilde_01_x1;
        this->biquad_tilde_01_x2 = __biquad_tilde_01_x2;
        this->biquad_tilde_01_y1 = __biquad_tilde_01_y1;
        this->biquad_tilde_01_y2 = __biquad_tilde_01_y2;
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void numberobj_01_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_01_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_01_value = localvalue;
    }
    
    void numberobj_02_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_02_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_02_value = localvalue;
    }
    
    void numberobj_03_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_03_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_03_value = localvalue;
    }
    
    void biquad_tilde_01_reset() {
        this->biquad_tilde_01_x1 = 0;
        this->biquad_tilde_01_x2 = 0;
        this->biquad_tilde_01_y1 = 0;
        this->biquad_tilde_01_y2 = 0;
    }
    
    void biquad_tilde_01_dspsetup(bool force) {
        if ((bool)(this->biquad_tilde_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->biquad_tilde_01_reset();
        this->biquad_tilde_01_setupDone = true;
    }
    
    void biquad_tilde_02_reset() {
        this->biquad_tilde_02_x1 = 0;
        this->biquad_tilde_02_x2 = 0;
        this->biquad_tilde_02_y1 = 0;
        this->biquad_tilde_02_y2 = 0;
    }
    
    void biquad_tilde_02_dspsetup(bool force) {
        if ((bool)(this->biquad_tilde_02_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->biquad_tilde_02_reset();
        this->biquad_tilde_02_setupDone = true;
    }
    
    array<number, 5> filtercoeff_01_localop_next(number frequency, number q, number gain, number type) {
        number omega = frequency * this->filtercoeff_01_localop_twopi_over_sr;
        this->filtercoeff_01_localop_cs = rnbo_cos(omega);
        this->filtercoeff_01_localop_sn = rnbo_sin(omega);
        this->filtercoeff_01_localop_one_over_gain = (gain >= 0 ? (number)1 / gain : 0.0);
        this->filtercoeff_01_localop_one_over_q = (number)1 / q;
        this->filtercoeff_01_localop_alpha = this->filtercoeff_01_localop_sn * 0.5 * this->filtercoeff_01_localop_one_over_q;
    
        switch ((int)type) {
        case 5:
            {
            this->filtercoeff_01_localop_A = this->safesqrt(gain);
    
            this->filtercoeff_01_localop_beta = this->safesqrt(
                (this->filtercoeff_01_localop_A * this->filtercoeff_01_localop_A + 1.) * this->filtercoeff_01_localop_one_over_q - (this->filtercoeff_01_localop_A - 1.) * (this->filtercoeff_01_localop_A - 1.)
            );
    
            this->filtercoeff_01_localop_b0 = (number)1 / (this->filtercoeff_01_localop_A + 1. + (this->filtercoeff_01_localop_A - 1.) * this->filtercoeff_01_localop_cs + this->filtercoeff_01_localop_beta * this->filtercoeff_01_localop_sn);
            break;
            }
        case 6:
            {
            this->filtercoeff_01_localop_A = this->safesqrt(gain);
    
            this->filtercoeff_01_localop_beta = this->safesqrt(
                (this->filtercoeff_01_localop_A * this->filtercoeff_01_localop_A + 1.) * this->filtercoeff_01_localop_one_over_q - (this->filtercoeff_01_localop_A - 1.) * (this->filtercoeff_01_localop_A - 1.)
            );
    
            this->filtercoeff_01_localop_b0 = (number)1 / (this->filtercoeff_01_localop_A + 1. - (this->filtercoeff_01_localop_A - 1.) * this->filtercoeff_01_localop_cs + this->filtercoeff_01_localop_beta * this->filtercoeff_01_localop_sn);
            break;
            }
        case 4:
            {
            this->filtercoeff_01_localop_A = this->safesqrt(gain);
            this->filtercoeff_01_localop_one_over_a = (this->filtercoeff_01_localop_A == 0 ? 0 : (number)1 / this->filtercoeff_01_localop_A);
            this->filtercoeff_01_localop_b0 = (number)1 / (1. + this->filtercoeff_01_localop_alpha * this->filtercoeff_01_localop_one_over_a);
            break;
            }
        case 9:
        case 10:
        case 11:
        case 13:
        case 14:
            {
            this->filtercoeff_01_localop_b0 = (number)1 / (1. + this->filtercoeff_01_localop_alpha);
            this->filtercoeff_01_localop_b0g = (number)1 / (this->filtercoeff_01_localop_one_over_gain + this->filtercoeff_01_localop_alpha * this->filtercoeff_01_localop_one_over_gain);
            break;
            }
        default:
            {
            this->filtercoeff_01_localop_b0 = (number)1 / (1. + this->filtercoeff_01_localop_alpha);
            break;
            }
        }
    
        switch ((int)type) {
        case 0:
            {
            this->filtercoeff_01_localop_la0 = this->filtercoeff_01_localop_la2 = (1. - this->filtercoeff_01_localop_cs) * 0.5 * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_la1 = (1. - this->filtercoeff_01_localop_cs) * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_lb1 = -2. * this->filtercoeff_01_localop_cs * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_lb2 = (1. - this->filtercoeff_01_localop_alpha) * this->filtercoeff_01_localop_b0;
            break;
            }
        case 1:
            {
            this->filtercoeff_01_localop_la0 = this->filtercoeff_01_localop_la2 = (1. + this->filtercoeff_01_localop_cs) * 0.5 * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_la1 = -(1. + this->filtercoeff_01_localop_cs) * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_lb1 = -2. * this->filtercoeff_01_localop_cs * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_lb2 = (1. - this->filtercoeff_01_localop_alpha) * this->filtercoeff_01_localop_b0;
            break;
            }
        case 2:
            {
            this->filtercoeff_01_localop_la0 = this->filtercoeff_01_localop_alpha * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_la1 = 0.;
            this->filtercoeff_01_localop_la2 = -this->filtercoeff_01_localop_alpha * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_lb1 = -2. * this->filtercoeff_01_localop_cs * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_lb2 = (1. - this->filtercoeff_01_localop_alpha) * this->filtercoeff_01_localop_b0;
            break;
            }
        case 7:
            {
            this->filtercoeff_01_localop_la0 = this->filtercoeff_01_localop_alpha * q * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_la1 = 0.;
            this->filtercoeff_01_localop_la2 = -this->filtercoeff_01_localop_alpha * q * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_lb1 = -2. * this->filtercoeff_01_localop_cs * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_lb2 = (1. - this->filtercoeff_01_localop_alpha) * this->filtercoeff_01_localop_b0;
            break;
            }
        case 3:
            {
            this->filtercoeff_01_localop_la1 = this->filtercoeff_01_localop_lb1 = -2. * this->filtercoeff_01_localop_cs * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_lb2 = (1. - this->filtercoeff_01_localop_alpha) * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_la0 = this->filtercoeff_01_localop_la2 = this->filtercoeff_01_localop_b0;
            break;
            }
        case 8:
            {
            this->filtercoeff_01_localop_la1 = this->filtercoeff_01_localop_lb1 = -2. * this->filtercoeff_01_localop_cs * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_lb2 = this->filtercoeff_01_localop_la0 = (1. - this->filtercoeff_01_localop_alpha) * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_la2 = 1.0;
            break;
            }
        case 4:
            {
            this->filtercoeff_01_localop_la0 = (1. + this->filtercoeff_01_localop_alpha * this->filtercoeff_01_localop_A) * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_la1 = this->filtercoeff_01_localop_lb1 = -2. * this->filtercoeff_01_localop_cs * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_la2 = (1. - this->filtercoeff_01_localop_alpha * this->filtercoeff_01_localop_A) * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_lb2 = (1. - this->filtercoeff_01_localop_alpha * this->filtercoeff_01_localop_one_over_a) * this->filtercoeff_01_localop_b0;
            break;
            }
        case 5:
            {
            this->filtercoeff_01_localop_la0 = this->filtercoeff_01_localop_A * (this->filtercoeff_01_localop_A + 1. - (this->filtercoeff_01_localop_A - 1.) * this->filtercoeff_01_localop_cs + this->filtercoeff_01_localop_beta * this->filtercoeff_01_localop_sn) * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_la1 = 2. * this->filtercoeff_01_localop_A * (this->filtercoeff_01_localop_A - 1 - (this->filtercoeff_01_localop_A + 1) * this->filtercoeff_01_localop_cs) * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_la2 = this->filtercoeff_01_localop_A * (this->filtercoeff_01_localop_A + 1. - (this->filtercoeff_01_localop_A - 1.) * this->filtercoeff_01_localop_cs - this->filtercoeff_01_localop_beta * this->filtercoeff_01_localop_sn) * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_lb1 = -2. * (this->filtercoeff_01_localop_A - 1. + (this->filtercoeff_01_localop_A + 1.) * this->filtercoeff_01_localop_cs) * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_lb2 = (this->filtercoeff_01_localop_A + 1. + (this->filtercoeff_01_localop_A - 1.) * this->filtercoeff_01_localop_cs - this->filtercoeff_01_localop_beta * this->filtercoeff_01_localop_sn) * this->filtercoeff_01_localop_b0;
            break;
            }
        case 6:
            {
            this->filtercoeff_01_localop_la0 = this->filtercoeff_01_localop_A * (this->filtercoeff_01_localop_A + 1. + (this->filtercoeff_01_localop_A - 1.) * this->filtercoeff_01_localop_cs + this->filtercoeff_01_localop_beta * this->filtercoeff_01_localop_sn) * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_la1 = -2. * this->filtercoeff_01_localop_A * (this->filtercoeff_01_localop_A - 1. + (this->filtercoeff_01_localop_A + 1.) * this->filtercoeff_01_localop_cs) * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_la2 = this->filtercoeff_01_localop_A * (this->filtercoeff_01_localop_A + 1. + (this->filtercoeff_01_localop_A - 1.) * this->filtercoeff_01_localop_cs - this->filtercoeff_01_localop_beta * this->filtercoeff_01_localop_sn) * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_lb1 = 2. * (this->filtercoeff_01_localop_A - 1. - (this->filtercoeff_01_localop_A + 1.) * this->filtercoeff_01_localop_cs) * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_lb2 = (this->filtercoeff_01_localop_A + 1. - (this->filtercoeff_01_localop_A - 1.) * this->filtercoeff_01_localop_cs - this->filtercoeff_01_localop_beta * this->filtercoeff_01_localop_sn) * this->filtercoeff_01_localop_b0;
            break;
            }
        case 9:
            {
            this->filtercoeff_01_localop_b0g = (number)1 / (this->filtercoeff_01_localop_one_over_gain + this->filtercoeff_01_localop_alpha * this->filtercoeff_01_localop_one_over_gain);
            this->filtercoeff_01_localop_la0 = this->filtercoeff_01_localop_la2 = (1. - this->filtercoeff_01_localop_cs) * 0.5 * this->filtercoeff_01_localop_b0g;
            this->filtercoeff_01_localop_la1 = (1. - this->filtercoeff_01_localop_cs) * this->filtercoeff_01_localop_b0g;
            this->filtercoeff_01_localop_lb1 = -2. * this->filtercoeff_01_localop_cs * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_lb2 = (1. - this->filtercoeff_01_localop_alpha) * this->filtercoeff_01_localop_b0;
            break;
            }
        case 10:
            {
            this->filtercoeff_01_localop_b0g = (number)1 / (this->filtercoeff_01_localop_one_over_gain + this->filtercoeff_01_localop_alpha * this->filtercoeff_01_localop_one_over_gain);
            this->filtercoeff_01_localop_la0 = this->filtercoeff_01_localop_la2 = (1. + this->filtercoeff_01_localop_cs) * 0.5 * this->filtercoeff_01_localop_b0g;
            this->filtercoeff_01_localop_la1 = -(1. + this->filtercoeff_01_localop_cs) * this->filtercoeff_01_localop_b0g;
            this->filtercoeff_01_localop_lb1 = -2. * this->filtercoeff_01_localop_cs * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_lb2 = (1. - this->filtercoeff_01_localop_alpha) * this->filtercoeff_01_localop_b0;
            break;
            }
        case 11:
            {
            this->filtercoeff_01_localop_la0 = this->filtercoeff_01_localop_alpha * gain * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_la1 = 0.;
            this->filtercoeff_01_localop_la2 = -this->filtercoeff_01_localop_alpha * gain * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_lb1 = -2. * this->filtercoeff_01_localop_cs * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_lb2 = (1. - this->filtercoeff_01_localop_alpha) * this->filtercoeff_01_localop_b0;
            break;
            }
        case 13:
            {
            this->filtercoeff_01_localop_la0 = this->filtercoeff_01_localop_alpha * gain * q * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_la1 = 0.;
            this->filtercoeff_01_localop_la2 = -this->filtercoeff_01_localop_alpha * gain * q * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_lb1 = -2. * this->filtercoeff_01_localop_cs * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_lb2 = (1. - this->filtercoeff_01_localop_alpha) * this->filtercoeff_01_localop_b0;
            break;
            }
        case 12:
            {
            this->filtercoeff_01_localop_b0g = (number)1 / (this->filtercoeff_01_localop_one_over_gain + this->filtercoeff_01_localop_alpha * this->filtercoeff_01_localop_one_over_gain);
            this->filtercoeff_01_localop_la1 = this->filtercoeff_01_localop_lb1 = -2. * this->filtercoeff_01_localop_cs;
            this->filtercoeff_01_localop_lb2 = (1. - this->filtercoeff_01_localop_alpha) * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_la1 *= this->filtercoeff_01_localop_b0g;
            this->filtercoeff_01_localop_lb1 *= this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_la0 = this->filtercoeff_01_localop_b0g;
            this->filtercoeff_01_localop_la2 = this->filtercoeff_01_localop_b0g;
            break;
            }
        case 14:
            {
            this->filtercoeff_01_localop_b0g = (number)1 / (this->filtercoeff_01_localop_one_over_gain + this->filtercoeff_01_localop_alpha * this->filtercoeff_01_localop_one_over_gain);
            this->filtercoeff_01_localop_la0 = (1. - this->filtercoeff_01_localop_alpha) * this->filtercoeff_01_localop_b0g;
            this->filtercoeff_01_localop_la1 = -2. * this->filtercoeff_01_localop_cs * this->filtercoeff_01_localop_b0g;
            this->filtercoeff_01_localop_la2 = gain;
            this->filtercoeff_01_localop_lb1 = -2. * this->filtercoeff_01_localop_cs * this->filtercoeff_01_localop_b0;
            this->filtercoeff_01_localop_lb2 = (1. - this->filtercoeff_01_localop_alpha) * this->filtercoeff_01_localop_b0;
            break;
            }
        case 15:
            {
            this->filtercoeff_01_localop_la0 = 1;
            this->filtercoeff_01_localop_la1 = 0;
            this->filtercoeff_01_localop_la2 = 0;
            this->filtercoeff_01_localop_lb1 = 0;
            this->filtercoeff_01_localop_lb2 = 0;
            }
        default:
            {
            break;
            }
        }
    
        return {
            this->filtercoeff_01_localop_la0,
            this->filtercoeff_01_localop_la1,
            this->filtercoeff_01_localop_la2,
            this->filtercoeff_01_localop_lb1,
            this->filtercoeff_01_localop_lb2
        };
    }
    
    void filtercoeff_01_localop_dspsetup() {
        this->filtercoeff_01_localop_twopi_over_sr = (number)6.283185307179586 / this->sr;
    }
    
    void filtercoeff_01_localop_reset() {
        this->filtercoeff_01_localop_twopi_over_sr = 0;
        this->filtercoeff_01_localop_cs = 0;
        this->filtercoeff_01_localop_sn = 0;
        this->filtercoeff_01_localop_one_over_gain = 0;
        this->filtercoeff_01_localop_one_over_q = 0;
        this->filtercoeff_01_localop_alpha = 0;
        this->filtercoeff_01_localop_beta = 0;
        this->filtercoeff_01_localop_b0 = 0;
        this->filtercoeff_01_localop_b0g = 0;
        this->filtercoeff_01_localop_A = 0;
        this->filtercoeff_01_localop_one_over_a = 0;
        this->filtercoeff_01_localop_la0 = 0;
        this->filtercoeff_01_localop_la1 = 0;
        this->filtercoeff_01_localop_la2 = 0;
        this->filtercoeff_01_localop_lb1 = 0;
        this->filtercoeff_01_localop_lb2 = 0;
    }
    
    void filtercoeff_01_updateTerms(number local_frequency, number local_gain, number local_q) {
        if ((bool)(this->filtercoeff_01_force_update) || local_frequency != this->filtercoeff_01_last_frequency || local_q != this->filtercoeff_01_last_q || local_gain != this->filtercoeff_01_last_gain || this->filtercoeff_01_type != this->filtercoeff_01_last_type) {
            array<number, 5> tmp = this->filtercoeff_01_localop_next(local_frequency, local_q, local_gain, this->filtercoeff_01_type);
            this->filtercoeff_01_la0 = tmp[0];
            this->filtercoeff_01_la1 = tmp[1];
            this->filtercoeff_01_la2 = tmp[2];
            this->filtercoeff_01_lb1 = tmp[3];
            this->filtercoeff_01_lb2 = tmp[4];
            this->filtercoeff_01_last_frequency = local_frequency;
            this->filtercoeff_01_last_q = local_q;
            this->filtercoeff_01_last_gain = local_gain;
            this->filtercoeff_01_last_type = this->filtercoeff_01_type;
            this->filtercoeff_01_force_update = false;
        }
    }
    
    void filtercoeff_01_dspsetup(bool force) {
        if ((bool)(this->filtercoeff_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        {
            this->filtercoeff_01_activeResamp = this->vectorsize();
        }
    
        this->filtercoeff_01_resamp_counter = 0;
        this->filtercoeff_01_la0 = 0.;
        this->filtercoeff_01_la1 = 0.;
        this->filtercoeff_01_la2 = 0.;
        this->filtercoeff_01_lb1 = 0.;
        this->filtercoeff_01_lb2 = 0.;
        this->filtercoeff_01_last_frequency = -1.;
        this->filtercoeff_01_last_q = -1.;
        this->filtercoeff_01_last_gain = -1.;
        this->filtercoeff_01_last_type = this->filtercoeff_01_type;
        this->filtercoeff_01_force_update = true;
        this->filtercoeff_01_setupDone = true;
        this->filtercoeff_01_localop_dspsetup();
    }
    
    void numberobj_01_init() {
        this->numberobj_01_currentFormat = 6;
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("Bassshelf/number_obj-3"), 1, this->_currentTime);
    }
    
    void numberobj_01_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_01_value;
    }
    
    void numberobj_01_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_01_value_set(preset["value"]);
    }
    
    void numberobj_02_init() {
        this->numberobj_02_currentFormat = 6;
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("Bassshelf/number_obj-4"), 1, this->_currentTime);
    }
    
    void numberobj_02_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_02_value;
    }
    
    void numberobj_02_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_02_value_set(preset["value"]);
    }
    
    void change_01_init() {
        this->change_01_prev = this->change_01_input;
    }
    
    void message_01_init() {
        this->message_01_set_set({1.2});
    }
    
    void numberobj_03_init() {
        this->numberobj_03_currentFormat = 0;
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("Bassshelf/number_obj-8"), 1, this->_currentTime);
    }
    
    void numberobj_03_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_03_value;
    }
    
    void numberobj_03_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_03_value_set(preset["value"]);
    }
    
    number numbertilde_01_smooth_d_next(number x) {
        number temp = (number)(x - this->numbertilde_01_smooth_d_prev);
        this->numbertilde_01_smooth_d_prev = x;
        return temp;
    }
    
    void numbertilde_01_smooth_d_dspsetup() {
        this->numbertilde_01_smooth_d_reset();
    }
    
    void numbertilde_01_smooth_d_reset() {
        this->numbertilde_01_smooth_d_prev = 0;
    }
    
    number numbertilde_01_smooth_next(number x, number up, number down) {
        if (this->numbertilde_01_smooth_d_next(x) != 0.) {
            if (x > this->numbertilde_01_smooth_prev) {
                number _up = up;
    
                if (_up < 1)
                    _up = 1;
    
                this->numbertilde_01_smooth_index = _up;
                this->numbertilde_01_smooth_increment = (x - this->numbertilde_01_smooth_prev) / _up;
            } else if (x < this->numbertilde_01_smooth_prev) {
                number _down = down;
    
                if (_down < 1)
                    _down = 1;
    
                this->numbertilde_01_smooth_index = _down;
                this->numbertilde_01_smooth_increment = (x - this->numbertilde_01_smooth_prev) / _down;
            }
        }
    
        if (this->numbertilde_01_smooth_index > 0) {
            this->numbertilde_01_smooth_prev += this->numbertilde_01_smooth_increment;
            this->numbertilde_01_smooth_index -= 1;
        } else {
            this->numbertilde_01_smooth_prev = x;
        }
    
        return this->numbertilde_01_smooth_prev;
    }
    
    void numbertilde_01_smooth_reset() {
        this->numbertilde_01_smooth_prev = 0;
        this->numbertilde_01_smooth_index = 0;
        this->numbertilde_01_smooth_increment = 0;
        this->numbertilde_01_smooth_d_reset();
    }
    
    void numbertilde_01_init() {
        this->numbertilde_01_currentMode = 1;
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("Bassshelf/number~_obj-7"), 1, this->_currentTime);
    }
    
    void numbertilde_01_dspsetup(bool force) {
        if ((bool)(this->numbertilde_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->numbertilde_01_currentIntervalInSamples = this->mstosamps(100);
        this->numbertilde_01_currentInterval = this->numbertilde_01_currentIntervalInSamples;
        this->numbertilde_01_rampInSamples = this->mstosamps(this->numbertilde_01_ramp);
        this->numbertilde_01_setupDone = true;
        this->numbertilde_01_smooth_d_dspsetup();
    }
    
    void message_02_init() {
        this->message_02_set_set({0.74});
    }
    
    bool stackprotect_check() {
        this->stackprotect_count++;
    
        if (this->stackprotect_count > 128) {
            console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
            return true;
        }
    
        return false;
    }
    
    void updateTime(MillisecondTime time) {
        this->_currentTime = time;
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));
    
        if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
            this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;
    
        if (this->sampleOffsetIntoNextAudioBuffer < 0)
            this->sampleOffsetIntoNextAudioBuffer = 0;
    }
    
    void assign_defaults()
    {
        biquad_tilde_01_x = 0;
        biquad_tilde_01_a0 = 0;
        biquad_tilde_01_a1 = 0;
        biquad_tilde_01_a2 = 0;
        biquad_tilde_01_b1 = 0;
        biquad_tilde_01_b2 = 0;
        biquad_tilde_02_x = 0;
        biquad_tilde_02_a0 = 0;
        biquad_tilde_02_a1 = 0;
        biquad_tilde_02_a2 = 0;
        biquad_tilde_02_b1 = 0;
        biquad_tilde_02_b2 = 0;
        filtercoeff_01_frequency = 90;
        filtercoeff_01_gain = 1;
        filtercoeff_01_q = 0.7;
        filtercoeff_01_type = 5;
        numberobj_01_value = 0;
        numberobj_01_value_setter(numberobj_01_value);
        numberobj_02_value = 0;
        numberobj_02_value_setter(numberobj_02_value);
        expr_01_in1 = 0;
        expr_01_out1 = 0;
        linetilde_01_time = 10;
        linetilde_01_keepramp = 1;
        change_01_input = 0;
        change_01_out = 0;
        select_01_test1 = 0;
        select_01_test2 = 1;
        numberobj_03_value = 0;
        numberobj_03_value_setter(numberobj_03_value);
        expr_02_in1 = 0;
        expr_02_in2 = 0;
        expr_02_out1 = 0;
        linetilde_02_time = 10;
        linetilde_02_keepramp = 1;
        numbertilde_01_input_number = 0;
        numbertilde_01_ramp = 0;
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        signals[1] = nullptr;
        signals[2] = nullptr;
        signals[3] = nullptr;
        signals[4] = nullptr;
        signals[5] = nullptr;
        signals[6] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        biquad_tilde_01_x1 = 0;
        biquad_tilde_01_x2 = 0;
        biquad_tilde_01_y1 = 0;
        biquad_tilde_01_y2 = 0;
        biquad_tilde_01_setupDone = false;
        biquad_tilde_02_x1 = 0;
        biquad_tilde_02_x2 = 0;
        biquad_tilde_02_y1 = 0;
        biquad_tilde_02_y2 = 0;
        biquad_tilde_02_setupDone = false;
        filtercoeff_01_K_EPSILON = 1e-9;
        filtercoeff_01_setupDone = false;
        numberobj_01_currentFormat = 6;
        numberobj_01_lastValue = 0;
        numberobj_02_currentFormat = 6;
        numberobj_02_lastValue = 0;
        linetilde_01_currentValue = 1;
        numberobj_03_currentFormat = 6;
        numberobj_03_lastValue = 0;
        linetilde_02_currentValue = 1;
        numbertilde_01_currentInterval = 0;
        numbertilde_01_currentIntervalInSamples = 0;
        numbertilde_01_lastValue = 0;
        numbertilde_01_outValue = 0;
        numbertilde_01_rampInSamples = 0;
        numbertilde_01_currentMode = 0;
        numbertilde_01_smooth_d_prev = 0;
        numbertilde_01_smooth_prev = 0;
        numbertilde_01_smooth_index = 0;
        numbertilde_01_smooth_increment = 0;
        numbertilde_01_setupDone = false;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number biquad_tilde_01_x;
        number biquad_tilde_01_a0;
        number biquad_tilde_01_a1;
        number biquad_tilde_01_a2;
        number biquad_tilde_01_b1;
        number biquad_tilde_01_b2;
        number biquad_tilde_02_x;
        number biquad_tilde_02_a0;
        number biquad_tilde_02_a1;
        number biquad_tilde_02_a2;
        number biquad_tilde_02_b1;
        number biquad_tilde_02_b2;
        number filtercoeff_01_frequency;
        number filtercoeff_01_gain;
        number filtercoeff_01_q;
        Int filtercoeff_01_type;
        number numberobj_01_value;
        number numberobj_02_value;
        number expr_01_in1;
        number expr_01_out1;
        list linetilde_01_segments;
        number linetilde_01_time;
        number linetilde_01_keepramp;
        number change_01_input;
        number change_01_out;
        list message_01_set;
        number select_01_test1;
        number select_01_test2;
        number numberobj_03_value;
        number expr_02_in1;
        number expr_02_in2;
        number expr_02_out1;
        list linetilde_02_segments;
        number linetilde_02_time;
        number linetilde_02_keepramp;
        number numbertilde_01_input_number;
        number numbertilde_01_ramp;
        list message_02_set;
        MillisecondTime _currentTime;
        UInt64 audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[7];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        number biquad_tilde_01_x1;
        number biquad_tilde_01_x2;
        number biquad_tilde_01_y1;
        number biquad_tilde_01_y2;
        bool biquad_tilde_01_setupDone;
        number biquad_tilde_02_x1;
        number biquad_tilde_02_x2;
        number biquad_tilde_02_y1;
        number biquad_tilde_02_y2;
        bool biquad_tilde_02_setupDone;
        number filtercoeff_01_resamp_counter;
        number filtercoeff_01_activeResamp;
        number filtercoeff_01_K_EPSILON;
        number filtercoeff_01_la0;
        number filtercoeff_01_la1;
        number filtercoeff_01_la2;
        number filtercoeff_01_lb1;
        number filtercoeff_01_lb2;
        number filtercoeff_01_last_frequency;
        number filtercoeff_01_last_q;
        number filtercoeff_01_last_gain;
        Int filtercoeff_01_last_type;
        bool filtercoeff_01_force_update;
        number filtercoeff_01_localop_twopi_over_sr;
        number filtercoeff_01_localop_cs;
        number filtercoeff_01_localop_sn;
        number filtercoeff_01_localop_one_over_gain;
        number filtercoeff_01_localop_one_over_q;
        number filtercoeff_01_localop_alpha;
        number filtercoeff_01_localop_beta;
        number filtercoeff_01_localop_b0;
        number filtercoeff_01_localop_b0g;
        number filtercoeff_01_localop_A;
        number filtercoeff_01_localop_one_over_a;
        number filtercoeff_01_localop_la0;
        number filtercoeff_01_localop_la1;
        number filtercoeff_01_localop_la2;
        number filtercoeff_01_localop_lb1;
        number filtercoeff_01_localop_lb2;
        bool filtercoeff_01_setupDone;
        Int numberobj_01_currentFormat;
        number numberobj_01_lastValue;
        Int numberobj_02_currentFormat;
        number numberobj_02_lastValue;
        list linetilde_01_activeRamps;
        number linetilde_01_currentValue;
        number change_01_prev;
        Int numberobj_03_currentFormat;
        number numberobj_03_lastValue;
        list linetilde_02_activeRamps;
        number linetilde_02_currentValue;
        SampleIndex numbertilde_01_currentInterval;
        SampleIndex numbertilde_01_currentIntervalInSamples;
        number numbertilde_01_lastValue;
        number numbertilde_01_outValue;
        number numbertilde_01_rampInSamples;
        Int numbertilde_01_currentMode;
        number numbertilde_01_smooth_d_prev;
        number numbertilde_01_smooth_prev;
        number numbertilde_01_smooth_index;
        number numbertilde_01_smooth_increment;
        bool numbertilde_01_setupDone;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_06 : public PatcherInterfaceImpl {
    
    friend class Kanal42110;
    
    public:
    
    RNBOSubpatcher_06()
    {
    }
    
    ~RNBOSubpatcher_06()
    {
    }
    
    virtual Kanal42110* getPatcher() const {
        return static_cast<Kanal42110 *>(_parentPatcher);
    }
    
    Kanal42110* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
        getEngine()->flushClockEvents(this, -1621164530, false);
        getEngine()->flushClockEvents(this, -62043057, false);
        getEngine()->flushClockEvents(this, -1494586265, false);
    }
    
    inline number safesqrt(number num) {
        return (num > 0.0 ? rnbo_sqrt(num) : 0.0);
    }
    
    Index vectorsize() const {
        return this->vs;
    }
    
    SampleIndex currentsampletime() {
        return this->audioProcessSampleCount + this->sampleOffsetIntoNextAudioBuffer;
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    Index getNumMidiInputPorts() const {
        return 0;
    }
    
    void processMidiEvent(MillisecondTime , int , ConstByteArray , Index ) {}
    
    Index getNumMidiOutputPorts() const {
        return 0;
    }
    
    void process(
        const SampleValue * const* inputs,
        Index numInputs,
        SampleValue * const* outputs,
        Index numOutputs,
        Index n
    ) {
        this->vs = n;
        this->updateTime(this->getEngine()->getCurrentTime());
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        SampleValue * out2 = (numOutputs >= 2 && outputs[1] ? outputs[1] : this->dummyBuffer);
        const SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        const SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        this->linetilde_03_perform(this->signals[0], n);
        this->linetilde_04_perform(this->signals[1], n);
        this->numbertilde_02_perform(this->signals[1], this->dummyBuffer, n);
    
        this->filtercoeff_02_perform(
            this->filtercoeff_02_frequency,
            this->signals[0],
            this->signals[1],
            this->signals[2],
            this->signals[3],
            this->signals[4],
            this->signals[5],
            this->signals[6],
            n
        );
    
        this->biquad_tilde_04_perform(
            in2,
            this->signals[2],
            this->signals[3],
            this->signals[4],
            this->signals[5],
            this->signals[6],
            out2,
            n
        );
    
        this->biquad_tilde_03_perform(
            in1,
            this->signals[2],
            this->signals[3],
            this->signals[4],
            this->signals[5],
            this->signals[6],
            out1,
            n
        );
    
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 7; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
            this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
            this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
            this->didAllocateSignals = true;
        }
    
        const bool sampleRateChanged = sampleRate != this->sr;
        const bool maxvsChanged = maxBlockSize != this->maxvs;
        const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;
    
        if (sampleRateChanged || maxvsChanged) {
            this->vs = maxBlockSize;
            this->maxvs = maxBlockSize;
            this->sr = sampleRate;
            this->invsr = 1 / sampleRate;
        }
    
        this->numbertilde_02_dspsetup(forceDSPSetup);
        this->filtercoeff_02_dspsetup(forceDSPSetup);
        this->biquad_tilde_04_dspsetup(forceDSPSetup);
        this->biquad_tilde_03_dspsetup(forceDSPSetup);
    
        if (sampleRateChanged)
            this->onSampleRateChanged(sampleRate);
    }
    
    void setProbingTarget(MessageTag id) {
        switch (id) {
        default:
            {
            this->setProbingIndex(-1);
            break;
            }
        }
    }
    
    void setProbingIndex(ProbingIndex ) {}
    
    Index getProbingChannels(MessageTag outletId) const {
        RNBO_UNUSED(outletId);
        return 0;
    }
    
    Index getIsMuted()  {
        return this->isMuted;
    }
    
    void setIsMuted(Index v)  {
        this->isMuted = v;
    }
    
    void onSampleRateChanged(double ) {}
    
    Index getPatcherSerial() const {
        return 0;
    }
    
    void getState(PatcherStateInterface& ) {}
    
    void setState() {}
    
    void getPreset(PatcherStateInterface& ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processParameterBangEvent(ParameterIndex index, MillisecondTime time) {
        this->setParameterValue(index, this->getParameterValue(index), time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
        default:
            {
            return 0;
            }
        }
    }
    
    ParameterIndex getNumSignalInParameters() const {
        return 0;
    }
    
    ParameterIndex getNumSignalOutParameters() const {
        return 0;
    }
    
    ParameterIndex getNumParameters() const {
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            {
            return "bogus";
            }
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            {
            return "bogus";
            }
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
    }
    
    ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
        if (steps == 1) {
            if (normalizedValue > 0) {
                normalizedValue = 1.;
            }
        } else {
            ParameterValue oneStep = (number)1. / (steps - 1);
            ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
            normalizedValue = numberOfSteps * oneStep;
        }
    
        return normalizedValue;
    }
    
    ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
    }
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case -1621164530:
            {
            this->linetilde_03_target_bang();
            break;
            }
        case -62043057:
            {
            this->linetilde_04_target_bang();
            break;
            }
        case -1494586265:
            {
            this->numbertilde_02_value_set(value);
            break;
            }
        }
    }
    
    void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}
    
    void processOutletEvent(
        EngineLink* sender,
        OutletIndex index,
        ParameterValue value,
        MillisecondTime time
    ) {
        this->updateTime(time);
        this->processOutletAtCurrentTime(sender, index, value);
    }
    
    void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("valin"):
            {
            if (TAG("Highshelf/number_obj-3") == objectId)
                this->numberobj_04_valin_set(payload);
    
            if (TAG("Highshelf/number_obj-4") == objectId)
                this->numberobj_05_valin_set(payload);
    
            if (TAG("Highshelf/number_obj-8") == objectId)
                this->numberobj_06_valin_set(payload);
    
            break;
            }
        case TAG("format"):
            {
            if (TAG("Highshelf/number_obj-3") == objectId)
                this->numberobj_04_format_set(payload);
    
            if (TAG("Highshelf/number_obj-4") == objectId)
                this->numberobj_05_format_set(payload);
    
            if (TAG("Highshelf/number_obj-8") == objectId)
                this->numberobj_06_format_set(payload);
    
            break;
            }
        case TAG("listin"):
            {
            if (TAG("Highshelf/message_obj-33") == objectId)
                this->message_03_listin_number_set(payload);
    
            if (TAG("Highshelf/message_obj-31") == objectId)
                this->message_04_listin_number_set(payload);
    
            break;
            }
        case TAG("sig"):
            {
            if (TAG("Highshelf/number~_obj-7") == objectId)
                this->numbertilde_02_sig_number_set(payload);
    
            break;
            }
        case TAG("mode"):
            {
            if (TAG("Highshelf/number~_obj-7") == objectId)
                this->numbertilde_02_mode_set(payload);
    
            break;
            }
        }
    }
    
    void processListMessage(
        MessageTag tag,
        MessageTag objectId,
        MillisecondTime time,
        const list& payload
    ) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("listin"):
            {
            if (TAG("Highshelf/message_obj-33") == objectId)
                this->message_03_listin_list_set(payload);
    
            if (TAG("Highshelf/message_obj-31") == objectId)
                this->message_04_listin_list_set(payload);
    
            break;
            }
        case TAG("sig"):
            {
            if (TAG("Highshelf/number~_obj-7") == objectId)
                this->numbertilde_02_sig_list_set(payload);
    
            break;
            }
        }
    }
    
    void processBangMessage(MessageTag tag, MessageTag objectId, MillisecondTime time) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("listin"):
            {
            if (TAG("Highshelf/message_obj-33") == objectId)
                this->message_03_listin_bang_bang();
    
            if (TAG("Highshelf/message_obj-31") == objectId)
                this->message_04_listin_bang_bang();
    
            break;
            }
        }
    }
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("valout"):
            {
            return "valout";
            }
        case TAG("Highshelf/number_obj-3"):
            {
            return "Highshelf/number_obj-3";
            }
        case TAG("setup"):
            {
            return "setup";
            }
        case TAG("Highshelf/number_obj-4"):
            {
            return "Highshelf/number_obj-4";
            }
        case TAG("listout"):
            {
            return "listout";
            }
        case TAG("Highshelf/message_obj-33"):
            {
            return "Highshelf/message_obj-33";
            }
        case TAG("Highshelf/number_obj-8"):
            {
            return "Highshelf/number_obj-8";
            }
        case TAG("Highshelf/message_obj-31"):
            {
            return "Highshelf/message_obj-31";
            }
        case TAG("monitor"):
            {
            return "monitor";
            }
        case TAG("Highshelf/number~_obj-7"):
            {
            return "Highshelf/number~_obj-7";
            }
        case TAG("assign"):
            {
            return "assign";
            }
        case TAG("valin"):
            {
            return "valin";
            }
        case TAG("format"):
            {
            return "format";
            }
        case TAG("listin"):
            {
            return "listin";
            }
        case TAG("sig"):
            {
            return "sig";
            }
        case TAG("mode"):
            {
            return "mode";
            }
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        default:
            {
            return nullptr;
            }
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 0;
    }
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void processDataViewUpdate(DataRefIndex , MillisecondTime ) {}
    
    void initialize() {
        this->assign_defaults();
        this->setState();
    }
    
    protected:
    
    void eventinlet_02_out1_bang_bang() {
        this->numberobj_05_value_bang();
    }
    
    void eventinlet_02_out1_list_set(const list& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_05_value_set(converted);
        }
    }
    
    void numberobj_04_valin_set(number v) {
        this->numberobj_04_value_set(v);
    }
    
    void numberobj_04_format_set(number v) {
        this->numberobj_04_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
    }
    
    void numberobj_05_valin_set(number v) {
        this->numberobj_05_value_set(v);
    }
    
    void numberobj_05_format_set(number v) {
        this->numberobj_05_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
    }
    
    void message_03_listin_list_set(const list& v) {
        this->message_03_set_set(v);
    }
    
    void message_03_listin_number_set(number v) {
        this->message_03_set_set(v);
    }
    
    void message_03_listin_bang_bang() {
        this->message_03_trigger_bang();
    }
    
    void numberobj_06_valin_set(number v) {
        this->numberobj_06_value_set(v);
    }
    
    void numberobj_06_format_set(number v) {
        this->numberobj_06_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
    }
    
    void message_04_listin_list_set(const list& v) {
        this->message_04_set_set(v);
    }
    
    void message_04_listin_number_set(number v) {
        this->message_04_set_set(v);
    }
    
    void message_04_listin_bang_bang() {
        this->message_04_trigger_bang();
    }
    
    void numbertilde_02_sig_number_set(number v) {
        this->numbertilde_02_outValue = v;
    }
    
    void numbertilde_02_sig_list_set(const list& v) {
        this->numbertilde_02_outValue = v[0];
    }
    
    void numbertilde_02_mode_set(number v) {
        if (v == 1) {
            this->numbertilde_02_currentMode = 0;
        } else if (v == 2) {
            this->numbertilde_02_currentMode = 1;
        }
    }
    
    void linetilde_03_target_bang() {}
    
    void linetilde_04_target_bang() {}
    
    void numbertilde_02_value_set(number ) {}
    
    number msToSamps(MillisecondTime ms, number sampleRate) {
        return ms * sampleRate * 0.001;
    }
    
    MillisecondTime sampsToMs(SampleIndex samps) {
        return samps * (this->invsr * 1000);
    }
    
    Index getMaxBlockSize() const {
        return this->maxvs;
    }
    
    number getSampleRate() const {
        return this->sr;
    }
    
    bool hasFixedVectorSize() const {
        return false;
    }
    
    Index getNumInputChannels() const {
        return 2;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {
        this->numberobj_04_init();
        this->numberobj_05_init();
        this->change_02_init();
        this->message_03_init();
        this->numberobj_06_init();
        this->message_04_init();
        this->numbertilde_02_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {}
    
    void change_02_zero_set(number ) {}
    
    void change_02_nonzero_set(number ) {}
    
    void linetilde_04_time_set(number v) {
        this->linetilde_04_time = v;
    }
    
    void linetilde_04_segments_set(const list& v) {
        this->linetilde_04_segments = jsCreateListCopy(v);
    
        if ((bool)(v->length)) {
            if (v->length == 1 && this->linetilde_04_time == 0) {
                this->linetilde_04_activeRamps->length = 0;
                this->linetilde_04_currentValue = v[0];
            } else {
                auto currentTime = this->currentsampletime();
                number lastRampValue = this->linetilde_04_currentValue;
                number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;
    
                for (Index i = 0; i < this->linetilde_04_activeRamps->length; i += 3) {
                    rampEnd = this->linetilde_04_activeRamps[(Index)(i + 2)];
    
                    if (rampEnd > currentTime) {
                        this->linetilde_04_activeRamps[(Index)(i + 2)] = currentTime;
                        number diff = rampEnd - currentTime;
                        number valueDiff = diff * this->linetilde_04_activeRamps[(Index)(i + 1)];
                        lastRampValue = this->linetilde_04_activeRamps[(Index)i] - valueDiff;
                        this->linetilde_04_activeRamps[(Index)i] = lastRampValue;
                        this->linetilde_04_activeRamps->length = i + 3;
                        rampEnd = currentTime;
                    } else {
                        lastRampValue = this->linetilde_04_activeRamps[(Index)i];
                    }
                }
    
                if (rampEnd < currentTime) {
                    this->linetilde_04_activeRamps->push(lastRampValue);
                    this->linetilde_04_activeRamps->push(0);
                    this->linetilde_04_activeRamps->push(currentTime);
                }
    
                number lastRampEnd = currentTime;
    
                for (Index i = 0; i < v->length; i += 2) {
                    number destinationValue = v[(Index)i];
                    number inc = 0;
                    number rampTimeInSamples;
    
                    if (v->length > i + 1) {
                        rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);
    
                        if ((bool)(this->linetilde_04_keepramp)) {
                            this->linetilde_04_time_set(v[(Index)(i + 1)]);
                        }
                    } else {
                        rampTimeInSamples = this->mstosamps(this->linetilde_04_time);
                    }
    
                    if (rampTimeInSamples <= 0) {
                        rampTimeInSamples = 1;
                    }
    
                    inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                    lastRampEnd += rampTimeInSamples;
                    this->linetilde_04_activeRamps->push(destinationValue);
                    this->linetilde_04_activeRamps->push(inc);
                    this->linetilde_04_activeRamps->push(lastRampEnd);
                    lastRampValue = destinationValue;
                }
            }
        }
    }
    
    void message_03_out_set(const list& v) {
        this->linetilde_04_segments_set(v);
    }
    
    void message_03_trigger_bang() {
        this->message_03_out_set(this->message_03_set);
    }
    
    void select_02_match1_bang() {
        this->message_03_trigger_bang();
    }
    
    void message_04_out_set(const list& v) {
        this->linetilde_04_segments_set(v);
    }
    
    void message_04_trigger_bang() {
        this->message_04_out_set(this->message_04_set);
    }
    
    void select_02_match2_bang() {
        this->message_04_trigger_bang();
    }
    
    void select_02_nomatch_number_set(number ) {}
    
    void select_02_input_number_set(number v) {
        if (v == this->select_02_test1)
            this->select_02_match1_bang();
        else if (v == this->select_02_test2)
            this->select_02_match2_bang();
        else
            this->select_02_nomatch_number_set(v);
    }
    
    void change_02_out_set(number v) {
        this->change_02_out = v;
        this->select_02_input_number_set(v);
    }
    
    void change_02_input_set(number v) {
        this->change_02_input = v;
    
        if (v != this->change_02_prev) {
            number prev = this->change_02_prev;
            this->change_02_prev = v;
    
            if (v == 0) {
                this->change_02_zero_set(1);
            } else if (this->change_02_out == 0) {
                this->change_02_nonzero_set(1);
            }
    
            {
                this->change_02_out_set(v);
            }
        } else {
            this->change_02_prev = v;
        }
    }
    
    void numberobj_06_output_set(number v) {
        this->change_02_input_set(v);
    }
    
    void numberobj_06_value_set(number v) {
        this->numberobj_06_value_setter(v);
        v = this->numberobj_06_value;
        number localvalue = v;
    
        if (this->numberobj_06_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("Highshelf/number_obj-8"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_06_output_set(localvalue);
    }
    
    void expr_04_out1_set(number v) {
        this->expr_04_out1 = v;
        this->numberobj_06_value_set(this->expr_04_out1);
    }
    
    void expr_04_in1_set(number in1) {
        this->expr_04_in1 = in1;
        this->expr_04_out1_set(this->expr_04_in1 > this->expr_04_in2);//#map:Highshelf/>_obj-2:1
    }
    
    void linetilde_03_time_set(number v) {
        this->linetilde_03_time = v;
    }
    
    void linetilde_03_segments_set(const list& v) {
        this->linetilde_03_segments = jsCreateListCopy(v);
    
        if ((bool)(v->length)) {
            if (v->length == 1 && this->linetilde_03_time == 0) {
                this->linetilde_03_activeRamps->length = 0;
                this->linetilde_03_currentValue = v[0];
            } else {
                auto currentTime = this->currentsampletime();
                number lastRampValue = this->linetilde_03_currentValue;
                number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;
    
                for (Index i = 0; i < this->linetilde_03_activeRamps->length; i += 3) {
                    rampEnd = this->linetilde_03_activeRamps[(Index)(i + 2)];
    
                    if (rampEnd > currentTime) {
                        this->linetilde_03_activeRamps[(Index)(i + 2)] = currentTime;
                        number diff = rampEnd - currentTime;
                        number valueDiff = diff * this->linetilde_03_activeRamps[(Index)(i + 1)];
                        lastRampValue = this->linetilde_03_activeRamps[(Index)i] - valueDiff;
                        this->linetilde_03_activeRamps[(Index)i] = lastRampValue;
                        this->linetilde_03_activeRamps->length = i + 3;
                        rampEnd = currentTime;
                    } else {
                        lastRampValue = this->linetilde_03_activeRamps[(Index)i];
                    }
                }
    
                if (rampEnd < currentTime) {
                    this->linetilde_03_activeRamps->push(lastRampValue);
                    this->linetilde_03_activeRamps->push(0);
                    this->linetilde_03_activeRamps->push(currentTime);
                }
    
                number lastRampEnd = currentTime;
    
                for (Index i = 0; i < v->length; i += 2) {
                    number destinationValue = v[(Index)i];
                    number inc = 0;
                    number rampTimeInSamples;
    
                    if (v->length > i + 1) {
                        rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);
    
                        if ((bool)(this->linetilde_03_keepramp)) {
                            this->linetilde_03_time_set(v[(Index)(i + 1)]);
                        }
                    } else {
                        rampTimeInSamples = this->mstosamps(this->linetilde_03_time);
                    }
    
                    if (rampTimeInSamples <= 0) {
                        rampTimeInSamples = 1;
                    }
    
                    inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                    lastRampEnd += rampTimeInSamples;
                    this->linetilde_03_activeRamps->push(destinationValue);
                    this->linetilde_03_activeRamps->push(inc);
                    this->linetilde_03_activeRamps->push(lastRampEnd);
                    lastRampValue = destinationValue;
                }
            }
        }
    }
    
    void numberobj_04_output_set(number v) {
        {
            list converted = {v};
            this->linetilde_03_segments_set(converted);
        }
    }
    
    void numberobj_04_value_set(number v) {
        this->numberobj_04_value_setter(v);
        v = this->numberobj_04_value;
        number localvalue = v;
    
        if (this->numberobj_04_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("Highshelf/number_obj-3"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_04_output_set(localvalue);
    }
    
    void expr_03_out1_set(number v) {
        this->expr_03_out1 = v;
        this->numberobj_04_value_set(this->expr_03_out1);
    }
    
    void expr_03_in1_set(number in1) {
        this->expr_03_in1 = in1;
        this->expr_03_out1_set(rnbo_pow(10, this->expr_03_in1 * 0.05));//#map:Highshelf/dbtoa_obj-9:1
    }
    
    void numberobj_05_output_set(number v) {
        this->expr_04_in1_set(v);
        this->expr_03_in1_set(v);
    }
    
    void numberobj_05_value_set(number v) {
        this->numberobj_05_value_setter(v);
        v = this->numberobj_05_value;
        number localvalue = v;
    
        if (this->numberobj_05_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("Highshelf/number_obj-4"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_05_output_set(localvalue);
    }
    
    void eventinlet_02_out1_number_set(number v) {
        this->numberobj_05_value_set(v);
    }
    
    void numberobj_05_value_bang() {
        number v = this->numberobj_05_value;
        number localvalue = v;
    
        if (this->numberobj_05_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("Highshelf/number_obj-4"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_05_output_set(localvalue);
    }
    
    void message_03_set_set(const list& v) {
        this->message_03_set = jsCreateListCopy(v);
        this->getEngine()->sendListMessage(TAG("listout"), TAG("Highshelf/message_obj-33"), v, this->_currentTime);
    }
    
    void message_04_set_set(const list& v) {
        this->message_04_set = jsCreateListCopy(v);
        this->getEngine()->sendListMessage(TAG("listout"), TAG("Highshelf/message_obj-31"), v, this->_currentTime);
    }
    
    void linetilde_03_perform(SampleValue * out, Index n) {
        auto __linetilde_03_time = this->linetilde_03_time;
        auto __linetilde_03_keepramp = this->linetilde_03_keepramp;
        auto __linetilde_03_currentValue = this->linetilde_03_currentValue;
        Index i = 0;
    
        if ((bool)(this->linetilde_03_activeRamps->length)) {
            while ((bool)(this->linetilde_03_activeRamps->length) && i < n) {
                number destinationValue = this->linetilde_03_activeRamps[0];
                number inc = this->linetilde_03_activeRamps[1];
                number rampTimeInSamples = this->linetilde_03_activeRamps[2] - this->audioProcessSampleCount - i;
                number val = __linetilde_03_currentValue;
    
                while (rampTimeInSamples > 0 && i < n) {
                    out[(Index)i] = val;
                    val += inc;
                    i++;
                    rampTimeInSamples--;
                }
    
                if (rampTimeInSamples <= 0) {
                    val = destinationValue;
                    this->linetilde_03_activeRamps->splice(0, 3);
    
                    if ((bool)(!(bool)(this->linetilde_03_activeRamps->length))) {
                        this->getEngine()->scheduleClockEventWithValue(
                            this,
                            -1621164530,
                            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                            0
                        );;
    
                        if ((bool)(!(bool)(__linetilde_03_keepramp))) {
                            __linetilde_03_time = 0;
                        }
                    }
                }
    
                __linetilde_03_currentValue = val;
            }
        }
    
        while (i < n) {
            out[(Index)i] = __linetilde_03_currentValue;
            i++;
        }
    
        this->linetilde_03_currentValue = __linetilde_03_currentValue;
        this->linetilde_03_time = __linetilde_03_time;
    }
    
    void linetilde_04_perform(SampleValue * out, Index n) {
        auto __linetilde_04_time = this->linetilde_04_time;
        auto __linetilde_04_keepramp = this->linetilde_04_keepramp;
        auto __linetilde_04_currentValue = this->linetilde_04_currentValue;
        Index i = 0;
    
        if ((bool)(this->linetilde_04_activeRamps->length)) {
            while ((bool)(this->linetilde_04_activeRamps->length) && i < n) {
                number destinationValue = this->linetilde_04_activeRamps[0];
                number inc = this->linetilde_04_activeRamps[1];
                number rampTimeInSamples = this->linetilde_04_activeRamps[2] - this->audioProcessSampleCount - i;
                number val = __linetilde_04_currentValue;
    
                while (rampTimeInSamples > 0 && i < n) {
                    out[(Index)i] = val;
                    val += inc;
                    i++;
                    rampTimeInSamples--;
                }
    
                if (rampTimeInSamples <= 0) {
                    val = destinationValue;
                    this->linetilde_04_activeRamps->splice(0, 3);
    
                    if ((bool)(!(bool)(this->linetilde_04_activeRamps->length))) {
                        this->getEngine()->scheduleClockEventWithValue(
                            this,
                            -62043057,
                            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                            0
                        );;
    
                        if ((bool)(!(bool)(__linetilde_04_keepramp))) {
                            __linetilde_04_time = 0;
                        }
                    }
                }
    
                __linetilde_04_currentValue = val;
            }
        }
    
        while (i < n) {
            out[(Index)i] = __linetilde_04_currentValue;
            i++;
        }
    
        this->linetilde_04_currentValue = __linetilde_04_currentValue;
        this->linetilde_04_time = __linetilde_04_time;
    }
    
    void numbertilde_02_perform(const SampleValue * input_signal, SampleValue * output, Index n) {
        auto __numbertilde_02_currentIntervalInSamples = this->numbertilde_02_currentIntervalInSamples;
        auto __numbertilde_02_lastValue = this->numbertilde_02_lastValue;
        auto __numbertilde_02_currentInterval = this->numbertilde_02_currentInterval;
        auto __numbertilde_02_rampInSamples = this->numbertilde_02_rampInSamples;
        auto __numbertilde_02_outValue = this->numbertilde_02_outValue;
        auto __numbertilde_02_currentMode = this->numbertilde_02_currentMode;
        number monitorvalue = input_signal[0];
    
        for (Index i = 0; i < n; i++) {
            if (__numbertilde_02_currentMode == 0) {
                output[(Index)i] = this->numbertilde_02_smooth_next(
                    __numbertilde_02_outValue,
                    __numbertilde_02_rampInSamples,
                    __numbertilde_02_rampInSamples
                );
            } else {
                output[(Index)i] = input_signal[(Index)i];
            }
        }
    
        __numbertilde_02_currentInterval -= n;
    
        if (monitorvalue != __numbertilde_02_lastValue && __numbertilde_02_currentInterval <= 0) {
            __numbertilde_02_currentInterval = __numbertilde_02_currentIntervalInSamples;
    
            this->getEngine()->scheduleClockEventWithValue(
                this,
                -1494586265,
                this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                monitorvalue
            );;
    
            __numbertilde_02_lastValue = monitorvalue;
    
            this->getEngine()->sendListMessage(
                TAG("monitor"),
                TAG("Highshelf/number~_obj-7"),
                {monitorvalue},
                this->_currentTime
            );;
        }
    
        this->numbertilde_02_currentInterval = __numbertilde_02_currentInterval;
        this->numbertilde_02_lastValue = __numbertilde_02_lastValue;
    }
    
    void filtercoeff_02_perform(
        number frequency,
        const Sample * gain,
        const Sample * q,
        SampleValue * a0,
        SampleValue * a1,
        SampleValue * a2,
        SampleValue * b1,
        SampleValue * b2,
        Index n
    ) {
        RNBO_UNUSED(frequency);
        auto __filtercoeff_02_activeResamp = this->filtercoeff_02_activeResamp;
        auto __filtercoeff_02_resamp_counter = this->filtercoeff_02_resamp_counter;
        auto __filtercoeff_02_K_EPSILON = this->filtercoeff_02_K_EPSILON;
    
        for (Index i = 0; i < n; i++) {
            number local_q = q[(Index)i];
            number local_gain = gain[(Index)i];
    
            if (local_gain < 0)
                local_gain = 0;
    
            number local_frequency = 4500;
    
            if (local_q < __filtercoeff_02_K_EPSILON)
                local_q = __filtercoeff_02_K_EPSILON;
    
            local_frequency = (local_frequency > this->sr * 0.5 ? this->sr * 0.5 : (local_frequency < 1 ? 1 : local_frequency));
            __filtercoeff_02_resamp_counter--;
    
            if (__filtercoeff_02_resamp_counter <= 0) {
                __filtercoeff_02_resamp_counter = __filtercoeff_02_activeResamp;
                this->filtercoeff_02_updateTerms(local_frequency, local_gain, local_q);
            }
    
            a0[(Index)i] = this->filtercoeff_02_la0;
            a1[(Index)i] = this->filtercoeff_02_la1;
            a2[(Index)i] = this->filtercoeff_02_la2;
            b1[(Index)i] = this->filtercoeff_02_lb1;
            b2[(Index)i] = this->filtercoeff_02_lb2;
        }
    
        this->filtercoeff_02_resamp_counter = __filtercoeff_02_resamp_counter;
    }
    
    void biquad_tilde_04_perform(
        const Sample * x,
        const Sample * a0,
        const Sample * a1,
        const Sample * a2,
        const Sample * b1,
        const Sample * b2,
        SampleValue * out1,
        Index n
    ) {
        auto __biquad_tilde_04_y2 = this->biquad_tilde_04_y2;
        auto __biquad_tilde_04_y1 = this->biquad_tilde_04_y1;
        auto __biquad_tilde_04_x2 = this->biquad_tilde_04_x2;
        auto __biquad_tilde_04_x1 = this->biquad_tilde_04_x1;
        Index i;
    
        for (i = 0; i < n; i++) {
            number tmp = x[(Index)i] * a0[(Index)i] + __biquad_tilde_04_x1 * a1[(Index)i] + __biquad_tilde_04_x2 * a2[(Index)i] - (__biquad_tilde_04_y1 * b1[(Index)i] + __biquad_tilde_04_y2 * b2[(Index)i]);
            __biquad_tilde_04_x2 = __biquad_tilde_04_x1;
            __biquad_tilde_04_x1 = x[(Index)i];
            __biquad_tilde_04_y2 = __biquad_tilde_04_y1;
            __biquad_tilde_04_y1 = tmp;
            out1[(Index)i] = tmp;
        }
    
        this->biquad_tilde_04_x1 = __biquad_tilde_04_x1;
        this->biquad_tilde_04_x2 = __biquad_tilde_04_x2;
        this->biquad_tilde_04_y1 = __biquad_tilde_04_y1;
        this->biquad_tilde_04_y2 = __biquad_tilde_04_y2;
    }
    
    void biquad_tilde_03_perform(
        const Sample * x,
        const Sample * a0,
        const Sample * a1,
        const Sample * a2,
        const Sample * b1,
        const Sample * b2,
        SampleValue * out1,
        Index n
    ) {
        auto __biquad_tilde_03_y2 = this->biquad_tilde_03_y2;
        auto __biquad_tilde_03_y1 = this->biquad_tilde_03_y1;
        auto __biquad_tilde_03_x2 = this->biquad_tilde_03_x2;
        auto __biquad_tilde_03_x1 = this->biquad_tilde_03_x1;
        Index i;
    
        for (i = 0; i < n; i++) {
            number tmp = x[(Index)i] * a0[(Index)i] + __biquad_tilde_03_x1 * a1[(Index)i] + __biquad_tilde_03_x2 * a2[(Index)i] - (__biquad_tilde_03_y1 * b1[(Index)i] + __biquad_tilde_03_y2 * b2[(Index)i]);
            __biquad_tilde_03_x2 = __biquad_tilde_03_x1;
            __biquad_tilde_03_x1 = x[(Index)i];
            __biquad_tilde_03_y2 = __biquad_tilde_03_y1;
            __biquad_tilde_03_y1 = tmp;
            out1[(Index)i] = tmp;
        }
    
        this->biquad_tilde_03_x1 = __biquad_tilde_03_x1;
        this->biquad_tilde_03_x2 = __biquad_tilde_03_x2;
        this->biquad_tilde_03_y1 = __biquad_tilde_03_y1;
        this->biquad_tilde_03_y2 = __biquad_tilde_03_y2;
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void numberobj_04_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_04_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_04_value = localvalue;
    }
    
    void numberobj_05_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_05_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_05_value = localvalue;
    }
    
    void numberobj_06_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_06_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_06_value = localvalue;
    }
    
    void biquad_tilde_03_reset() {
        this->biquad_tilde_03_x1 = 0;
        this->biquad_tilde_03_x2 = 0;
        this->biquad_tilde_03_y1 = 0;
        this->biquad_tilde_03_y2 = 0;
    }
    
    void biquad_tilde_03_dspsetup(bool force) {
        if ((bool)(this->biquad_tilde_03_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->biquad_tilde_03_reset();
        this->biquad_tilde_03_setupDone = true;
    }
    
    void biquad_tilde_04_reset() {
        this->biquad_tilde_04_x1 = 0;
        this->biquad_tilde_04_x2 = 0;
        this->biquad_tilde_04_y1 = 0;
        this->biquad_tilde_04_y2 = 0;
    }
    
    void biquad_tilde_04_dspsetup(bool force) {
        if ((bool)(this->biquad_tilde_04_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->biquad_tilde_04_reset();
        this->biquad_tilde_04_setupDone = true;
    }
    
    array<number, 5> filtercoeff_02_localop_next(number frequency, number q, number gain, number type) {
        number omega = frequency * this->filtercoeff_02_localop_twopi_over_sr;
        this->filtercoeff_02_localop_cs = rnbo_cos(omega);
        this->filtercoeff_02_localop_sn = rnbo_sin(omega);
        this->filtercoeff_02_localop_one_over_gain = (gain >= 0 ? (number)1 / gain : 0.0);
        this->filtercoeff_02_localop_one_over_q = (number)1 / q;
        this->filtercoeff_02_localop_alpha = this->filtercoeff_02_localop_sn * 0.5 * this->filtercoeff_02_localop_one_over_q;
    
        switch ((int)type) {
        case 5:
            {
            this->filtercoeff_02_localop_A = this->safesqrt(gain);
    
            this->filtercoeff_02_localop_beta = this->safesqrt(
                (this->filtercoeff_02_localop_A * this->filtercoeff_02_localop_A + 1.) * this->filtercoeff_02_localop_one_over_q - (this->filtercoeff_02_localop_A - 1.) * (this->filtercoeff_02_localop_A - 1.)
            );
    
            this->filtercoeff_02_localop_b0 = (number)1 / (this->filtercoeff_02_localop_A + 1. + (this->filtercoeff_02_localop_A - 1.) * this->filtercoeff_02_localop_cs + this->filtercoeff_02_localop_beta * this->filtercoeff_02_localop_sn);
            break;
            }
        case 6:
            {
            this->filtercoeff_02_localop_A = this->safesqrt(gain);
    
            this->filtercoeff_02_localop_beta = this->safesqrt(
                (this->filtercoeff_02_localop_A * this->filtercoeff_02_localop_A + 1.) * this->filtercoeff_02_localop_one_over_q - (this->filtercoeff_02_localop_A - 1.) * (this->filtercoeff_02_localop_A - 1.)
            );
    
            this->filtercoeff_02_localop_b0 = (number)1 / (this->filtercoeff_02_localop_A + 1. - (this->filtercoeff_02_localop_A - 1.) * this->filtercoeff_02_localop_cs + this->filtercoeff_02_localop_beta * this->filtercoeff_02_localop_sn);
            break;
            }
        case 4:
            {
            this->filtercoeff_02_localop_A = this->safesqrt(gain);
            this->filtercoeff_02_localop_one_over_a = (this->filtercoeff_02_localop_A == 0 ? 0 : (number)1 / this->filtercoeff_02_localop_A);
            this->filtercoeff_02_localop_b0 = (number)1 / (1. + this->filtercoeff_02_localop_alpha * this->filtercoeff_02_localop_one_over_a);
            break;
            }
        case 9:
        case 10:
        case 11:
        case 13:
        case 14:
            {
            this->filtercoeff_02_localop_b0 = (number)1 / (1. + this->filtercoeff_02_localop_alpha);
            this->filtercoeff_02_localop_b0g = (number)1 / (this->filtercoeff_02_localop_one_over_gain + this->filtercoeff_02_localop_alpha * this->filtercoeff_02_localop_one_over_gain);
            break;
            }
        default:
            {
            this->filtercoeff_02_localop_b0 = (number)1 / (1. + this->filtercoeff_02_localop_alpha);
            break;
            }
        }
    
        switch ((int)type) {
        case 0:
            {
            this->filtercoeff_02_localop_la0 = this->filtercoeff_02_localop_la2 = (1. - this->filtercoeff_02_localop_cs) * 0.5 * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_la1 = (1. - this->filtercoeff_02_localop_cs) * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_lb1 = -2. * this->filtercoeff_02_localop_cs * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_lb2 = (1. - this->filtercoeff_02_localop_alpha) * this->filtercoeff_02_localop_b0;
            break;
            }
        case 1:
            {
            this->filtercoeff_02_localop_la0 = this->filtercoeff_02_localop_la2 = (1. + this->filtercoeff_02_localop_cs) * 0.5 * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_la1 = -(1. + this->filtercoeff_02_localop_cs) * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_lb1 = -2. * this->filtercoeff_02_localop_cs * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_lb2 = (1. - this->filtercoeff_02_localop_alpha) * this->filtercoeff_02_localop_b0;
            break;
            }
        case 2:
            {
            this->filtercoeff_02_localop_la0 = this->filtercoeff_02_localop_alpha * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_la1 = 0.;
            this->filtercoeff_02_localop_la2 = -this->filtercoeff_02_localop_alpha * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_lb1 = -2. * this->filtercoeff_02_localop_cs * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_lb2 = (1. - this->filtercoeff_02_localop_alpha) * this->filtercoeff_02_localop_b0;
            break;
            }
        case 7:
            {
            this->filtercoeff_02_localop_la0 = this->filtercoeff_02_localop_alpha * q * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_la1 = 0.;
            this->filtercoeff_02_localop_la2 = -this->filtercoeff_02_localop_alpha * q * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_lb1 = -2. * this->filtercoeff_02_localop_cs * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_lb2 = (1. - this->filtercoeff_02_localop_alpha) * this->filtercoeff_02_localop_b0;
            break;
            }
        case 3:
            {
            this->filtercoeff_02_localop_la1 = this->filtercoeff_02_localop_lb1 = -2. * this->filtercoeff_02_localop_cs * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_lb2 = (1. - this->filtercoeff_02_localop_alpha) * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_la0 = this->filtercoeff_02_localop_la2 = this->filtercoeff_02_localop_b0;
            break;
            }
        case 8:
            {
            this->filtercoeff_02_localop_la1 = this->filtercoeff_02_localop_lb1 = -2. * this->filtercoeff_02_localop_cs * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_lb2 = this->filtercoeff_02_localop_la0 = (1. - this->filtercoeff_02_localop_alpha) * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_la2 = 1.0;
            break;
            }
        case 4:
            {
            this->filtercoeff_02_localop_la0 = (1. + this->filtercoeff_02_localop_alpha * this->filtercoeff_02_localop_A) * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_la1 = this->filtercoeff_02_localop_lb1 = -2. * this->filtercoeff_02_localop_cs * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_la2 = (1. - this->filtercoeff_02_localop_alpha * this->filtercoeff_02_localop_A) * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_lb2 = (1. - this->filtercoeff_02_localop_alpha * this->filtercoeff_02_localop_one_over_a) * this->filtercoeff_02_localop_b0;
            break;
            }
        case 5:
            {
            this->filtercoeff_02_localop_la0 = this->filtercoeff_02_localop_A * (this->filtercoeff_02_localop_A + 1. - (this->filtercoeff_02_localop_A - 1.) * this->filtercoeff_02_localop_cs + this->filtercoeff_02_localop_beta * this->filtercoeff_02_localop_sn) * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_la1 = 2. * this->filtercoeff_02_localop_A * (this->filtercoeff_02_localop_A - 1 - (this->filtercoeff_02_localop_A + 1) * this->filtercoeff_02_localop_cs) * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_la2 = this->filtercoeff_02_localop_A * (this->filtercoeff_02_localop_A + 1. - (this->filtercoeff_02_localop_A - 1.) * this->filtercoeff_02_localop_cs - this->filtercoeff_02_localop_beta * this->filtercoeff_02_localop_sn) * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_lb1 = -2. * (this->filtercoeff_02_localop_A - 1. + (this->filtercoeff_02_localop_A + 1.) * this->filtercoeff_02_localop_cs) * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_lb2 = (this->filtercoeff_02_localop_A + 1. + (this->filtercoeff_02_localop_A - 1.) * this->filtercoeff_02_localop_cs - this->filtercoeff_02_localop_beta * this->filtercoeff_02_localop_sn) * this->filtercoeff_02_localop_b0;
            break;
            }
        case 6:
            {
            this->filtercoeff_02_localop_la0 = this->filtercoeff_02_localop_A * (this->filtercoeff_02_localop_A + 1. + (this->filtercoeff_02_localop_A - 1.) * this->filtercoeff_02_localop_cs + this->filtercoeff_02_localop_beta * this->filtercoeff_02_localop_sn) * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_la1 = -2. * this->filtercoeff_02_localop_A * (this->filtercoeff_02_localop_A - 1. + (this->filtercoeff_02_localop_A + 1.) * this->filtercoeff_02_localop_cs) * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_la2 = this->filtercoeff_02_localop_A * (this->filtercoeff_02_localop_A + 1. + (this->filtercoeff_02_localop_A - 1.) * this->filtercoeff_02_localop_cs - this->filtercoeff_02_localop_beta * this->filtercoeff_02_localop_sn) * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_lb1 = 2. * (this->filtercoeff_02_localop_A - 1. - (this->filtercoeff_02_localop_A + 1.) * this->filtercoeff_02_localop_cs) * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_lb2 = (this->filtercoeff_02_localop_A + 1. - (this->filtercoeff_02_localop_A - 1.) * this->filtercoeff_02_localop_cs - this->filtercoeff_02_localop_beta * this->filtercoeff_02_localop_sn) * this->filtercoeff_02_localop_b0;
            break;
            }
        case 9:
            {
            this->filtercoeff_02_localop_b0g = (number)1 / (this->filtercoeff_02_localop_one_over_gain + this->filtercoeff_02_localop_alpha * this->filtercoeff_02_localop_one_over_gain);
            this->filtercoeff_02_localop_la0 = this->filtercoeff_02_localop_la2 = (1. - this->filtercoeff_02_localop_cs) * 0.5 * this->filtercoeff_02_localop_b0g;
            this->filtercoeff_02_localop_la1 = (1. - this->filtercoeff_02_localop_cs) * this->filtercoeff_02_localop_b0g;
            this->filtercoeff_02_localop_lb1 = -2. * this->filtercoeff_02_localop_cs * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_lb2 = (1. - this->filtercoeff_02_localop_alpha) * this->filtercoeff_02_localop_b0;
            break;
            }
        case 10:
            {
            this->filtercoeff_02_localop_b0g = (number)1 / (this->filtercoeff_02_localop_one_over_gain + this->filtercoeff_02_localop_alpha * this->filtercoeff_02_localop_one_over_gain);
            this->filtercoeff_02_localop_la0 = this->filtercoeff_02_localop_la2 = (1. + this->filtercoeff_02_localop_cs) * 0.5 * this->filtercoeff_02_localop_b0g;
            this->filtercoeff_02_localop_la1 = -(1. + this->filtercoeff_02_localop_cs) * this->filtercoeff_02_localop_b0g;
            this->filtercoeff_02_localop_lb1 = -2. * this->filtercoeff_02_localop_cs * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_lb2 = (1. - this->filtercoeff_02_localop_alpha) * this->filtercoeff_02_localop_b0;
            break;
            }
        case 11:
            {
            this->filtercoeff_02_localop_la0 = this->filtercoeff_02_localop_alpha * gain * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_la1 = 0.;
            this->filtercoeff_02_localop_la2 = -this->filtercoeff_02_localop_alpha * gain * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_lb1 = -2. * this->filtercoeff_02_localop_cs * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_lb2 = (1. - this->filtercoeff_02_localop_alpha) * this->filtercoeff_02_localop_b0;
            break;
            }
        case 13:
            {
            this->filtercoeff_02_localop_la0 = this->filtercoeff_02_localop_alpha * gain * q * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_la1 = 0.;
            this->filtercoeff_02_localop_la2 = -this->filtercoeff_02_localop_alpha * gain * q * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_lb1 = -2. * this->filtercoeff_02_localop_cs * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_lb2 = (1. - this->filtercoeff_02_localop_alpha) * this->filtercoeff_02_localop_b0;
            break;
            }
        case 12:
            {
            this->filtercoeff_02_localop_b0g = (number)1 / (this->filtercoeff_02_localop_one_over_gain + this->filtercoeff_02_localop_alpha * this->filtercoeff_02_localop_one_over_gain);
            this->filtercoeff_02_localop_la1 = this->filtercoeff_02_localop_lb1 = -2. * this->filtercoeff_02_localop_cs;
            this->filtercoeff_02_localop_lb2 = (1. - this->filtercoeff_02_localop_alpha) * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_la1 *= this->filtercoeff_02_localop_b0g;
            this->filtercoeff_02_localop_lb1 *= this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_la0 = this->filtercoeff_02_localop_b0g;
            this->filtercoeff_02_localop_la2 = this->filtercoeff_02_localop_b0g;
            break;
            }
        case 14:
            {
            this->filtercoeff_02_localop_b0g = (number)1 / (this->filtercoeff_02_localop_one_over_gain + this->filtercoeff_02_localop_alpha * this->filtercoeff_02_localop_one_over_gain);
            this->filtercoeff_02_localop_la0 = (1. - this->filtercoeff_02_localop_alpha) * this->filtercoeff_02_localop_b0g;
            this->filtercoeff_02_localop_la1 = -2. * this->filtercoeff_02_localop_cs * this->filtercoeff_02_localop_b0g;
            this->filtercoeff_02_localop_la2 = gain;
            this->filtercoeff_02_localop_lb1 = -2. * this->filtercoeff_02_localop_cs * this->filtercoeff_02_localop_b0;
            this->filtercoeff_02_localop_lb2 = (1. - this->filtercoeff_02_localop_alpha) * this->filtercoeff_02_localop_b0;
            break;
            }
        case 15:
            {
            this->filtercoeff_02_localop_la0 = 1;
            this->filtercoeff_02_localop_la1 = 0;
            this->filtercoeff_02_localop_la2 = 0;
            this->filtercoeff_02_localop_lb1 = 0;
            this->filtercoeff_02_localop_lb2 = 0;
            }
        default:
            {
            break;
            }
        }
    
        return {
            this->filtercoeff_02_localop_la0,
            this->filtercoeff_02_localop_la1,
            this->filtercoeff_02_localop_la2,
            this->filtercoeff_02_localop_lb1,
            this->filtercoeff_02_localop_lb2
        };
    }
    
    void filtercoeff_02_localop_dspsetup() {
        this->filtercoeff_02_localop_twopi_over_sr = (number)6.283185307179586 / this->sr;
    }
    
    void filtercoeff_02_localop_reset() {
        this->filtercoeff_02_localop_twopi_over_sr = 0;
        this->filtercoeff_02_localop_cs = 0;
        this->filtercoeff_02_localop_sn = 0;
        this->filtercoeff_02_localop_one_over_gain = 0;
        this->filtercoeff_02_localop_one_over_q = 0;
        this->filtercoeff_02_localop_alpha = 0;
        this->filtercoeff_02_localop_beta = 0;
        this->filtercoeff_02_localop_b0 = 0;
        this->filtercoeff_02_localop_b0g = 0;
        this->filtercoeff_02_localop_A = 0;
        this->filtercoeff_02_localop_one_over_a = 0;
        this->filtercoeff_02_localop_la0 = 0;
        this->filtercoeff_02_localop_la1 = 0;
        this->filtercoeff_02_localop_la2 = 0;
        this->filtercoeff_02_localop_lb1 = 0;
        this->filtercoeff_02_localop_lb2 = 0;
    }
    
    void filtercoeff_02_updateTerms(number local_frequency, number local_gain, number local_q) {
        if ((bool)(this->filtercoeff_02_force_update) || local_frequency != this->filtercoeff_02_last_frequency || local_q != this->filtercoeff_02_last_q || local_gain != this->filtercoeff_02_last_gain || this->filtercoeff_02_type != this->filtercoeff_02_last_type) {
            array<number, 5> tmp = this->filtercoeff_02_localop_next(local_frequency, local_q, local_gain, this->filtercoeff_02_type);
            this->filtercoeff_02_la0 = tmp[0];
            this->filtercoeff_02_la1 = tmp[1];
            this->filtercoeff_02_la2 = tmp[2];
            this->filtercoeff_02_lb1 = tmp[3];
            this->filtercoeff_02_lb2 = tmp[4];
            this->filtercoeff_02_last_frequency = local_frequency;
            this->filtercoeff_02_last_q = local_q;
            this->filtercoeff_02_last_gain = local_gain;
            this->filtercoeff_02_last_type = this->filtercoeff_02_type;
            this->filtercoeff_02_force_update = false;
        }
    }
    
    void filtercoeff_02_dspsetup(bool force) {
        if ((bool)(this->filtercoeff_02_setupDone) && (bool)(!(bool)(force)))
            return;
    
        {
            this->filtercoeff_02_activeResamp = this->vectorsize();
        }
    
        this->filtercoeff_02_resamp_counter = 0;
        this->filtercoeff_02_la0 = 0.;
        this->filtercoeff_02_la1 = 0.;
        this->filtercoeff_02_la2 = 0.;
        this->filtercoeff_02_lb1 = 0.;
        this->filtercoeff_02_lb2 = 0.;
        this->filtercoeff_02_last_frequency = -1.;
        this->filtercoeff_02_last_q = -1.;
        this->filtercoeff_02_last_gain = -1.;
        this->filtercoeff_02_last_type = this->filtercoeff_02_type;
        this->filtercoeff_02_force_update = true;
        this->filtercoeff_02_setupDone = true;
        this->filtercoeff_02_localop_dspsetup();
    }
    
    void numberobj_04_init() {
        this->numberobj_04_currentFormat = 6;
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("Highshelf/number_obj-3"), 1, this->_currentTime);
    }
    
    void numberobj_04_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_04_value;
    }
    
    void numberobj_04_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_04_value_set(preset["value"]);
    }
    
    void numberobj_05_init() {
        this->numberobj_05_currentFormat = 6;
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("Highshelf/number_obj-4"), 1, this->_currentTime);
    }
    
    void numberobj_05_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_05_value;
    }
    
    void numberobj_05_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_05_value_set(preset["value"]);
    }
    
    void change_02_init() {
        this->change_02_prev = this->change_02_input;
    }
    
    void message_03_init() {
        this->message_03_set_set({0.85});
    }
    
    void numberobj_06_init() {
        this->numberobj_06_currentFormat = 0;
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("Highshelf/number_obj-8"), 1, this->_currentTime);
    }
    
    void numberobj_06_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_06_value;
    }
    
    void numberobj_06_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_06_value_set(preset["value"]);
    }
    
    void message_04_init() {
        this->message_04_set_set({0.45});
    }
    
    number numbertilde_02_smooth_d_next(number x) {
        number temp = (number)(x - this->numbertilde_02_smooth_d_prev);
        this->numbertilde_02_smooth_d_prev = x;
        return temp;
    }
    
    void numbertilde_02_smooth_d_dspsetup() {
        this->numbertilde_02_smooth_d_reset();
    }
    
    void numbertilde_02_smooth_d_reset() {
        this->numbertilde_02_smooth_d_prev = 0;
    }
    
    number numbertilde_02_smooth_next(number x, number up, number down) {
        if (this->numbertilde_02_smooth_d_next(x) != 0.) {
            if (x > this->numbertilde_02_smooth_prev) {
                number _up = up;
    
                if (_up < 1)
                    _up = 1;
    
                this->numbertilde_02_smooth_index = _up;
                this->numbertilde_02_smooth_increment = (x - this->numbertilde_02_smooth_prev) / _up;
            } else if (x < this->numbertilde_02_smooth_prev) {
                number _down = down;
    
                if (_down < 1)
                    _down = 1;
    
                this->numbertilde_02_smooth_index = _down;
                this->numbertilde_02_smooth_increment = (x - this->numbertilde_02_smooth_prev) / _down;
            }
        }
    
        if (this->numbertilde_02_smooth_index > 0) {
            this->numbertilde_02_smooth_prev += this->numbertilde_02_smooth_increment;
            this->numbertilde_02_smooth_index -= 1;
        } else {
            this->numbertilde_02_smooth_prev = x;
        }
    
        return this->numbertilde_02_smooth_prev;
    }
    
    void numbertilde_02_smooth_reset() {
        this->numbertilde_02_smooth_prev = 0;
        this->numbertilde_02_smooth_index = 0;
        this->numbertilde_02_smooth_increment = 0;
        this->numbertilde_02_smooth_d_reset();
    }
    
    void numbertilde_02_init() {
        this->numbertilde_02_currentMode = 1;
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("Highshelf/number~_obj-7"), 1, this->_currentTime);
    }
    
    void numbertilde_02_dspsetup(bool force) {
        if ((bool)(this->numbertilde_02_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->numbertilde_02_currentIntervalInSamples = this->mstosamps(100);
        this->numbertilde_02_currentInterval = this->numbertilde_02_currentIntervalInSamples;
        this->numbertilde_02_rampInSamples = this->mstosamps(this->numbertilde_02_ramp);
        this->numbertilde_02_setupDone = true;
        this->numbertilde_02_smooth_d_dspsetup();
    }
    
    bool stackprotect_check() {
        this->stackprotect_count++;
    
        if (this->stackprotect_count > 128) {
            console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
            return true;
        }
    
        return false;
    }
    
    void updateTime(MillisecondTime time) {
        this->_currentTime = time;
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));
    
        if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
            this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;
    
        if (this->sampleOffsetIntoNextAudioBuffer < 0)
            this->sampleOffsetIntoNextAudioBuffer = 0;
    }
    
    void assign_defaults()
    {
        biquad_tilde_03_x = 0;
        biquad_tilde_03_a0 = 0;
        biquad_tilde_03_a1 = 0;
        biquad_tilde_03_a2 = 0;
        biquad_tilde_03_b1 = 0;
        biquad_tilde_03_b2 = 0;
        biquad_tilde_04_x = 0;
        biquad_tilde_04_a0 = 0;
        biquad_tilde_04_a1 = 0;
        biquad_tilde_04_a2 = 0;
        biquad_tilde_04_b1 = 0;
        biquad_tilde_04_b2 = 0;
        filtercoeff_02_frequency = 4500;
        filtercoeff_02_gain = 1;
        filtercoeff_02_q = 0.7;
        filtercoeff_02_type = 6;
        numberobj_04_value = 0;
        numberobj_04_value_setter(numberobj_04_value);
        numberobj_05_value = 0;
        numberobj_05_value_setter(numberobj_05_value);
        expr_03_in1 = 0;
        expr_03_out1 = 0;
        linetilde_03_time = 10;
        linetilde_03_keepramp = 1;
        change_02_input = 0;
        change_02_out = 0;
        select_02_test1 = 0;
        select_02_test2 = 1;
        numberobj_06_value = 0;
        numberobj_06_value_setter(numberobj_06_value);
        expr_04_in1 = 0;
        expr_04_in2 = 0;
        expr_04_out1 = 0;
        linetilde_04_time = 10;
        linetilde_04_keepramp = 1;
        numbertilde_02_input_number = 0;
        numbertilde_02_ramp = 0;
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        signals[1] = nullptr;
        signals[2] = nullptr;
        signals[3] = nullptr;
        signals[4] = nullptr;
        signals[5] = nullptr;
        signals[6] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        biquad_tilde_03_x1 = 0;
        biquad_tilde_03_x2 = 0;
        biquad_tilde_03_y1 = 0;
        biquad_tilde_03_y2 = 0;
        biquad_tilde_03_setupDone = false;
        biquad_tilde_04_x1 = 0;
        biquad_tilde_04_x2 = 0;
        biquad_tilde_04_y1 = 0;
        biquad_tilde_04_y2 = 0;
        biquad_tilde_04_setupDone = false;
        filtercoeff_02_K_EPSILON = 1e-9;
        filtercoeff_02_setupDone = false;
        numberobj_04_currentFormat = 6;
        numberobj_04_lastValue = 0;
        numberobj_05_currentFormat = 6;
        numberobj_05_lastValue = 0;
        linetilde_03_currentValue = 1;
        numberobj_06_currentFormat = 6;
        numberobj_06_lastValue = 0;
        linetilde_04_currentValue = 1;
        numbertilde_02_currentInterval = 0;
        numbertilde_02_currentIntervalInSamples = 0;
        numbertilde_02_lastValue = 0;
        numbertilde_02_outValue = 0;
        numbertilde_02_rampInSamples = 0;
        numbertilde_02_currentMode = 0;
        numbertilde_02_smooth_d_prev = 0;
        numbertilde_02_smooth_prev = 0;
        numbertilde_02_smooth_index = 0;
        numbertilde_02_smooth_increment = 0;
        numbertilde_02_setupDone = false;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number biquad_tilde_03_x;
        number biquad_tilde_03_a0;
        number biquad_tilde_03_a1;
        number biquad_tilde_03_a2;
        number biquad_tilde_03_b1;
        number biquad_tilde_03_b2;
        number biquad_tilde_04_x;
        number biquad_tilde_04_a0;
        number biquad_tilde_04_a1;
        number biquad_tilde_04_a2;
        number biquad_tilde_04_b1;
        number biquad_tilde_04_b2;
        number filtercoeff_02_frequency;
        number filtercoeff_02_gain;
        number filtercoeff_02_q;
        Int filtercoeff_02_type;
        number numberobj_04_value;
        number numberobj_05_value;
        number expr_03_in1;
        number expr_03_out1;
        list linetilde_03_segments;
        number linetilde_03_time;
        number linetilde_03_keepramp;
        number change_02_input;
        number change_02_out;
        list message_03_set;
        number select_02_test1;
        number select_02_test2;
        number numberobj_06_value;
        number expr_04_in1;
        number expr_04_in2;
        number expr_04_out1;
        list linetilde_04_segments;
        number linetilde_04_time;
        number linetilde_04_keepramp;
        list message_04_set;
        number numbertilde_02_input_number;
        number numbertilde_02_ramp;
        MillisecondTime _currentTime;
        UInt64 audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[7];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        number biquad_tilde_03_x1;
        number biquad_tilde_03_x2;
        number biquad_tilde_03_y1;
        number biquad_tilde_03_y2;
        bool biquad_tilde_03_setupDone;
        number biquad_tilde_04_x1;
        number biquad_tilde_04_x2;
        number biquad_tilde_04_y1;
        number biquad_tilde_04_y2;
        bool biquad_tilde_04_setupDone;
        number filtercoeff_02_resamp_counter;
        number filtercoeff_02_activeResamp;
        number filtercoeff_02_K_EPSILON;
        number filtercoeff_02_la0;
        number filtercoeff_02_la1;
        number filtercoeff_02_la2;
        number filtercoeff_02_lb1;
        number filtercoeff_02_lb2;
        number filtercoeff_02_last_frequency;
        number filtercoeff_02_last_q;
        number filtercoeff_02_last_gain;
        Int filtercoeff_02_last_type;
        bool filtercoeff_02_force_update;
        number filtercoeff_02_localop_twopi_over_sr;
        number filtercoeff_02_localop_cs;
        number filtercoeff_02_localop_sn;
        number filtercoeff_02_localop_one_over_gain;
        number filtercoeff_02_localop_one_over_q;
        number filtercoeff_02_localop_alpha;
        number filtercoeff_02_localop_beta;
        number filtercoeff_02_localop_b0;
        number filtercoeff_02_localop_b0g;
        number filtercoeff_02_localop_A;
        number filtercoeff_02_localop_one_over_a;
        number filtercoeff_02_localop_la0;
        number filtercoeff_02_localop_la1;
        number filtercoeff_02_localop_la2;
        number filtercoeff_02_localop_lb1;
        number filtercoeff_02_localop_lb2;
        bool filtercoeff_02_setupDone;
        Int numberobj_04_currentFormat;
        number numberobj_04_lastValue;
        Int numberobj_05_currentFormat;
        number numberobj_05_lastValue;
        list linetilde_03_activeRamps;
        number linetilde_03_currentValue;
        number change_02_prev;
        Int numberobj_06_currentFormat;
        number numberobj_06_lastValue;
        list linetilde_04_activeRamps;
        number linetilde_04_currentValue;
        SampleIndex numbertilde_02_currentInterval;
        SampleIndex numbertilde_02_currentIntervalInSamples;
        number numbertilde_02_lastValue;
        number numbertilde_02_outValue;
        number numbertilde_02_rampInSamples;
        Int numbertilde_02_currentMode;
        number numbertilde_02_smooth_d_prev;
        number numbertilde_02_smooth_prev;
        number numbertilde_02_smooth_index;
        number numbertilde_02_smooth_increment;
        bool numbertilde_02_setupDone;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

Kanal42110()
{
}

~Kanal42110()
{
    delete this->p_01;
    delete this->p_02;
}

Kanal42110* getTopLevelPatcher() {
    return this;
}

void cancelClockEvents()
{
    getEngine()->flushClockEvents(this, 1646922831, false);
    getEngine()->flushClockEvents(this, -1580124982, false);
    getEngine()->flushClockEvents(this, 1920515087, false);
    getEngine()->flushClockEvents(this, -21003509, false);
    getEngine()->flushClockEvents(this, 1538117964, false);
    getEngine()->flushClockEvents(this, -1494586265, false);
    getEngine()->flushClockEvents(this, -1197727859, false);
    getEngine()->flushClockEvents(this, 361393614, false);
    getEngine()->flushClockEvents(this, -815330736, false);
    getEngine()->flushClockEvents(this, 743790737, false);
    getEngine()->flushClockEvents(this, 349887047, false);
    getEngine()->flushClockEvents(this, 1909008520, false);
    getEngine()->flushClockEvents(this, -826837303, false);
    getEngine()->flushClockEvents(this, 732284170, false);
    getEngine()->flushClockEvents(this, -2003561653, false);
    getEngine()->flushClockEvents(this, -444440180, false);
    getEngine()->flushClockEvents(this, 1114681293, false);
    getEngine()->flushClockEvents(this, -1621164530, false);
    getEngine()->flushClockEvents(this, -62043057, false);
}

template <typename T> void listquicksort(T& arr, T& sortindices, Int l, Int h, bool ascending) {
    if (l < h) {
        Int p = (Int)(this->listpartition(arr, sortindices, l, h, ascending));
        this->listquicksort(arr, sortindices, l, p - 1, ascending);
        this->listquicksort(arr, sortindices, p + 1, h, ascending);
    }
}

template <typename T> Int listpartition(T& arr, T& sortindices, Int l, Int h, bool ascending) {
    number x = arr[(Index)h];
    Int i = (Int)(l - 1);

    for (Int j = (Int)(l); j <= h - 1; j++) {
        bool asc = (bool)((bool)(ascending) && arr[(Index)j] <= x);
        bool desc = (bool)((bool)(!(bool)(ascending)) && arr[(Index)j] >= x);

        if ((bool)(asc) || (bool)(desc)) {
            i++;
            this->listswapelements(arr, i, j);
            this->listswapelements(sortindices, i, j);
        }
    }

    i++;
    this->listswapelements(arr, i, h);
    this->listswapelements(sortindices, i, h);
    return i;
}

template <typename T> void listswapelements(T& arr, Int a, Int b) {
    auto tmp = arr[(Index)a];
    arr[(Index)a] = arr[(Index)b];
    arr[(Index)b] = tmp;
}

number samplerate() const {
    return this->sr;
}

number maximum(number x, number y) {
    return (x < y ? y : x);
}

number minimum(number x, number y) {
    return (y < x ? y : x);
}

number __wrapped_op_clamp(number in1, number in2, number in3) {
    return (in1 > in3 ? in3 : (in1 < in2 ? in2 : in1));
}

SampleIndex currentsampletime() {
    return this->audioProcessSampleCount + this->sampleOffsetIntoNextAudioBuffer;
}

number mstosamps(MillisecondTime ms) {
    return ms * this->sr * 0.001;
}

inline number safediv(number num, number denom) {
    return (denom == 0.0 ? 0.0 : num / denom);
}

number safepow(number base, number exponent) {
    return fixnan(rnbo_pow(base, exponent));
}

number scale(
    number x,
    number lowin,
    number hiin,
    number lowout,
    number highout,
    number pow
) {
    auto inscale = this->safediv(1., hiin - lowin);
    number outdiff = highout - lowout;
    number value = (x - lowin) * inscale;

    if (pow != 1) {
        if (value > 0)
            value = this->safepow(value, pow);
        else
            value = -this->safepow(-value, pow);
    }

    value = value * outdiff + lowout;
    return value;
}

MillisecondTime sampstoms(number samps) {
    return samps * 1000 / this->sr;
}

Index getNumMidiInputPorts() const {
    return 0;
}

void processMidiEvent(MillisecondTime , int , ConstByteArray , Index ) {}

Index getNumMidiOutputPorts() const {
    return 0;
}

void process(
    const SampleValue * const* inputs,
    Index numInputs,
    SampleValue * const* outputs,
    Index numOutputs,
    Index n
) {
    this->vs = n;
    this->updateTime(this->getEngine()->getCurrentTime());
    SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
    SampleValue * out2 = (numOutputs >= 2 && outputs[1] ? outputs[1] : this->dummyBuffer);
    const SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
    const SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
    this->linetilde_05_perform(this->signals[0], n);
    this->linetilde_06_perform(this->signals[1], n);
    this->dspexpr_03_perform(in1, this->signals[1], this->signals[2], n);
    this->dspexpr_07_perform(in2, this->signals[1], this->signals[3], n);
    this->dspexpr_13_perform(this->signals[2], this->signals[3], this->signals[1], n);
    this->dspexpr_12_perform(this->signals[1], this->dspexpr_12_in2, this->signals[4], n);
    this->dspexpr_15_perform(this->signals[4], this->signals[1], n);

    this->slide_tilde_01_perform(
        this->signals[1],
        this->slide_tilde_01_up,
        this->slide_tilde_01_down,
        this->signals[4],
        n
    );

    this->dspexpr_14_perform(this->signals[4], this->signals[1], n);
    this->snapshot_01_perform(this->signals[1], n);
    this->linetilde_07_perform(this->signals[1], n);
    this->linetilde_08_perform(this->signals[4], n);
    this->numbertilde_03_perform(this->signals[4], this->dummyBuffer, n);
    this->linetilde_09_perform(this->signals[5], n);
    this->ip_01_perform(this->signals[6], n);
    this->linetilde_10_perform(this->signals[7], n);
    this->dspexpr_11_perform(this->signals[5], this->signals[6], this->signals[7], this->signals[8], n);
    this->linetilde_11_perform(this->signals[7], n);
    this->linetilde_12_perform(this->signals[6], n);
    this->linetilde_13_perform(this->signals[5], n);
    this->linetilde_14_perform(this->signals[9], n);
    this->linetilde_15_perform(this->signals[10], n);
    this->linetilde_16_perform(this->signals[11], n);
    this->linetilde_17_perform(this->signals[12], n);

    this->gen_01_perform(
        this->signals[2],
        this->signals[3],
        this->signals[11],
        this->signals[12],
        this->signals[13],
        this->signals[14],
        n
    );

    this->dspexpr_04_perform(this->signals[13], this->signals[1], this->signals[12], n);
    this->dspexpr_10_perform(this->signals[14], this->signals[1], this->signals[13], n);
    this->linetilde_18_perform(this->signals[1], n);
    this->linetilde_19_perform(this->signals[14], n);
    this->linetilde_20_perform(this->signals[11], n);
    this->linetilde_21_perform(this->signals[3], n);

    this->gen_03_perform(
        this->signals[12],
        this->signals[13],
        this->signals[7],
        this->signals[6],
        this->signals[5],
        this->signals[9],
        this->signals[10],
        this->signals[14],
        this->signals[11],
        this->signals[3],
        this->signals[2],
        this->signals[15],
        n
    );

    this->gen_02_perform(
        this->signals[2],
        this->signals[15],
        this->signals[1],
        this->signals[3],
        this->signals[11],
        n
    );

    this->p_01_perform(
        this->signals[3],
        this->signals[11],
        this->signals[1],
        this->signals[15],
        n
    );

    this->p_02_perform(
        this->signals[1],
        this->signals[15],
        this->signals[11],
        this->signals[3],
        n
    );

    this->dspexpr_05_perform(this->signals[11], this->signals[0], this->signals[15], n);
    this->dspexpr_06_perform(this->signals[15], this->signals[4], this->signals[11], n);
    this->onepole_tilde_01_perform(this->signals[11], this->onepole_tilde_01_freqInHz, this->signals[15], n);
    this->dspexpr_01_perform(in1, this->signals[15], this->signals[8], out1, n);
    this->dspexpr_08_perform(this->signals[3], this->signals[0], this->signals[15], n);
    this->dspexpr_09_perform(this->signals[15], this->signals[4], this->signals[0], n);
    this->onepole_tilde_02_perform(this->signals[0], this->onepole_tilde_02_freqInHz, this->signals[4], n);
    this->dspexpr_02_perform(in2, this->signals[4], this->signals[8], out2, n);
    this->stackprotect_perform(n);
    this->globaltransport_advance();
    this->audioProcessSampleCount += this->vs;
}

void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
    if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
        Index i;

        for (i = 0; i < 16; i++) {
            this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
        }

        this->ip_01_sigbuf = resizeSignal(this->ip_01_sigbuf, this->maxvs, maxBlockSize);
        this->globaltransport_tempo = resizeSignal(this->globaltransport_tempo, this->maxvs, maxBlockSize);
        this->globaltransport_state = resizeSignal(this->globaltransport_state, this->maxvs, maxBlockSize);
        this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
        this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
        this->didAllocateSignals = true;
    }

    const bool sampleRateChanged = sampleRate != this->sr;
    const bool maxvsChanged = maxBlockSize != this->maxvs;
    const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;

    if (sampleRateChanged || maxvsChanged) {
        this->vs = maxBlockSize;
        this->maxvs = maxBlockSize;
        this->sr = sampleRate;
        this->invsr = 1 / sampleRate;
    }

    this->numbertilde_03_dspsetup(forceDSPSetup);
    this->ip_01_dspsetup(forceDSPSetup);
    this->onepole_tilde_01_dspsetup(forceDSPSetup);
    this->onepole_tilde_02_dspsetup(forceDSPSetup);
    this->globaltransport_dspsetup(forceDSPSetup);
    this->p_01->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_02->prepareToProcess(sampleRate, maxBlockSize, force);

    if (sampleRateChanged)
        this->onSampleRateChanged(sampleRate);
}

void setProbingTarget(MessageTag id) {
    switch (id) {
    default:
        {
        this->setProbingIndex(-1);
        break;
        }
    }
}

void setProbingIndex(ProbingIndex ) {}

Index getProbingChannels(MessageTag outletId) const {
    RNBO_UNUSED(outletId);
    return 0;
}

DataRef* getDataRef(DataRefIndex index)  {
    switch (index) {
    default:
        {
        return nullptr;
        }
    }
}

DataRefIndex getNumDataRefs() const {
    return 0;
}

void fillDataRef(DataRefIndex , DataRef& ) {}

void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
    this->p_01->processDataViewUpdate(index, time);
    this->p_02->processDataViewUpdate(index, time);
}

void initialize() {
    this->assign_defaults();
    this->setState();
    this->initializeObjects();
    this->allocateDataRefs();
    this->startup();
}

Index getIsMuted()  {
    return this->isMuted;
}

void setIsMuted(Index v)  {
    this->isMuted = v;
}

void onSampleRateChanged(double ) {}

Index getPatcherSerial() const {
    return 0;
}

void getState(PatcherStateInterface& ) {}

void setState() {
    this->p_01 = new RNBOSubpatcher_05();
    this->p_01->setEngineAndPatcher(this->getEngine(), this);
    this->p_01->initialize();
    this->p_01->setParameterOffset(this->getParameterOffset(this->p_01));
    this->p_02 = new RNBOSubpatcher_06();
    this->p_02->setEngineAndPatcher(this->getEngine(), this);
    this->p_02->initialize();
    this->p_02->setParameterOffset(this->getParameterOffset(this->p_02));
}

void getPreset(PatcherStateInterface& preset) {
    preset["__presetid"] = "rnbo";
    this->param_01_getPresetValue(getSubState(preset, "Trim42"));
    this->param_02_getPresetValue(getSubState(preset, "WetLevel42"));
    this->param_03_getPresetValue(getSubState(preset, "Kanal116"));
    this->param_04_getPresetValue(getSubState(preset, "DryWet42"));
    this->param_05_getPresetValue(getSubState(preset, "Bypass42"));
    this->param_06_getPresetValue(getSubState(preset, "Treble42"));
    this->param_07_getPresetValue(getSubState(preset, "Bass42"));
    this->param_08_getPresetValue(getSubState(preset, "Drive42"));
    this->p_01->getPreset(getSubState(getSubState(preset, "__sps"), "Bassshelf"));
    this->p_02->getPreset(getSubState(getSubState(preset, "__sps"), "Highshelf"));
}

void setPreset(MillisecondTime time, PatcherStateInterface& preset) {
    this->updateTime(time);
    this->param_01_setPresetValue(getSubState(preset, "Trim42"));
    this->param_02_setPresetValue(getSubState(preset, "WetLevel42"));
    this->param_03_setPresetValue(getSubState(preset, "Kanal116"));
    this->param_04_setPresetValue(getSubState(preset, "DryWet42"));
    this->param_05_setPresetValue(getSubState(preset, "Bypass42"));
    this->param_06_setPresetValue(getSubState(preset, "Treble42"));
    this->param_07_setPresetValue(getSubState(preset, "Bass42"));
    this->param_08_setPresetValue(getSubState(preset, "Drive42"));
}

void setParameterValue(ParameterIndex index, ParameterValue v, MillisecondTime time) {
    this->updateTime(time);

    switch (index) {
    case 0:
        {
        this->param_01_value_set(v);
        break;
        }
    case 1:
        {
        this->param_02_value_set(v);
        break;
        }
    case 2:
        {
        this->param_03_value_set(v);
        break;
        }
    case 3:
        {
        this->param_04_value_set(v);
        break;
        }
    case 4:
        {
        this->param_05_value_set(v);
        break;
        }
    case 5:
        {
        this->param_06_value_set(v);
        break;
        }
    case 6:
        {
        this->param_07_value_set(v);
        break;
        }
    case 7:
        {
        this->param_08_value_set(v);
        break;
        }
    default:
        {
        index -= 8;

        if (index < this->p_01->getNumParameters())
            this->p_01->setParameterValue(index, v, time);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            this->p_02->setParameterValue(index, v, time);

        break;
        }
    }
}

void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
    this->setParameterValue(index, value, time);
}

void processParameterBangEvent(ParameterIndex index, MillisecondTime time) {
    this->setParameterValue(index, this->getParameterValue(index), time);
}

void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
    this->setParameterValueNormalized(index, value, time);
}

ParameterValue getParameterValue(ParameterIndex index)  {
    switch (index) {
    case 0:
        {
        return this->param_01_value;
        }
    case 1:
        {
        return this->param_02_value;
        }
    case 2:
        {
        return this->param_03_value;
        }
    case 3:
        {
        return this->param_04_value;
        }
    case 4:
        {
        return this->param_05_value;
        }
    case 5:
        {
        return this->param_06_value;
        }
    case 6:
        {
        return this->param_07_value;
        }
    case 7:
        {
        return this->param_08_value;
        }
    default:
        {
        index -= 8;

        if (index < this->p_01->getNumParameters())
            return this->p_01->getParameterValue(index);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->getParameterValue(index);

        return 0;
        }
    }
}

ParameterIndex getNumSignalInParameters() const {
    return 0;
}

ParameterIndex getNumSignalOutParameters() const {
    return 0;
}

ParameterIndex getNumParameters() const {
    return 8 + this->p_01->getNumParameters() + this->p_02->getNumParameters();
}

ConstCharPointer getParameterName(ParameterIndex index) const {
    switch (index) {
    case 0:
        {
        return "Trim42";
        }
    case 1:
        {
        return "WetLevel42";
        }
    case 2:
        {
        return "Kanal116";
        }
    case 3:
        {
        return "DryWet42";
        }
    case 4:
        {
        return "Bypass42";
        }
    case 5:
        {
        return "Treble42";
        }
    case 6:
        {
        return "Bass42";
        }
    case 7:
        {
        return "Drive42";
        }
    default:
        {
        index -= 8;

        if (index < this->p_01->getNumParameters())
            return this->p_01->getParameterName(index);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->getParameterName(index);

        return "bogus";
        }
    }
}

ConstCharPointer getParameterId(ParameterIndex index) const {
    switch (index) {
    case 0:
        {
        return "Trim42";
        }
    case 1:
        {
        return "WetLevel42";
        }
    case 2:
        {
        return "Kanal116";
        }
    case 3:
        {
        return "DryWet42";
        }
    case 4:
        {
        return "Bypass42";
        }
    case 5:
        {
        return "Treble42";
        }
    case 6:
        {
        return "Bass42";
        }
    case 7:
        {
        return "Drive42";
        }
    default:
        {
        index -= 8;

        if (index < this->p_01->getNumParameters())
            return this->p_01->getParameterId(index);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->getParameterId(index);

        return "bogus";
        }
    }
}

void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
    {
        switch (index) {
        case 0:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = -24;
            info->max = 24;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 1:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = -24;
            info->max = 24;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 2:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 1.16;
            info->min = 1;
            info->max = 2;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 3:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 1;
            info->min = 0;
            info->max = 1;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 4:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = 0;
            info->max = 1;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 5:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = -12;
            info->max = 12;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 6:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = -12;
            info->max = 12;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 7:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = 0;
            info->max = 1;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        default:
            {
            index -= 8;

            if (index < this->p_01->getNumParameters())
                this->p_01->getParameterInfo(index, info);

            index -= this->p_01->getNumParameters();

            if (index < this->p_02->getNumParameters())
                this->p_02->getParameterInfo(index, info);

            break;
            }
        }
    }
}

void sendParameter(ParameterIndex index, bool ignoreValue) {
    this->getEngine()->notifyParameterValueChanged(index, (ignoreValue ? 0 : this->getParameterValue(index)), ignoreValue);
}

ParameterIndex getParameterOffset(BaseInterface* subpatcher) const {
    if (subpatcher == this->p_01)
        return 8;

    if (subpatcher == this->p_02)
        return 8 + this->p_01->getNumParameters();

    return 0;
}

ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
    if (steps == 1) {
        if (normalizedValue > 0) {
            normalizedValue = 1.;
        }
    } else {
        ParameterValue oneStep = (number)1. / (steps - 1);
        ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
        normalizedValue = numberOfSteps * oneStep;
    }

    return normalizedValue;
}

ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
    switch (index) {
    case 3:
    case 4:
    case 7:
        {
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));
            ParameterValue normalizedValue = (value - 0) / (1 - 0);
            return normalizedValue;
        }
        }
    case 2:
        {
        {
            value = (value < 1 ? 1 : (value > 2 ? 2 : value));
            ParameterValue normalizedValue = (value - 1) / (2 - 1);
            return normalizedValue;
        }
        }
    case 0:
    case 1:
        {
        {
            value = (value < -24 ? -24 : (value > 24 ? 24 : value));
            ParameterValue normalizedValue = (value - -24) / (24 - -24);
            return normalizedValue;
        }
        }
    case 5:
    case 6:
        {
        {
            value = (value < -12 ? -12 : (value > 12 ? 12 : value));
            ParameterValue normalizedValue = (value - -12) / (12 - -12);
            return normalizedValue;
        }
        }
    default:
        {
        index -= 8;

        if (index < this->p_01->getNumParameters())
            return this->p_01->convertToNormalizedParameterValue(index, value);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->convertToNormalizedParameterValue(index, value);

        return value;
        }
    }
}

ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
    value = (value < 0 ? 0 : (value > 1 ? 1 : value));

    switch (index) {
    case 3:
    case 4:
    case 7:
        {
        {
            {
                return 0 + value * (1 - 0);
            }
        }
        }
    case 2:
        {
        {
            {
                return 1 + value * (2 - 1);
            }
        }
        }
    case 0:
    case 1:
        {
        {
            {
                return -24 + value * (24 - -24);
            }
        }
        }
    case 5:
    case 6:
        {
        {
            {
                return -12 + value * (12 - -12);
            }
        }
        }
    default:
        {
        index -= 8;

        if (index < this->p_01->getNumParameters())
            return this->p_01->convertFromNormalizedParameterValue(index, value);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->convertFromNormalizedParameterValue(index, value);

        return value;
        }
    }
}

ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
    switch (index) {
    case 0:
        {
        return this->param_01_value_constrain(value);
        }
    case 1:
        {
        return this->param_02_value_constrain(value);
        }
    case 2:
        {
        return this->param_03_value_constrain(value);
        }
    case 3:
        {
        return this->param_04_value_constrain(value);
        }
    case 4:
        {
        return this->param_05_value_constrain(value);
        }
    case 5:
        {
        return this->param_06_value_constrain(value);
        }
    case 6:
        {
        return this->param_07_value_constrain(value);
        }
    case 7:
        {
        return this->param_08_value_constrain(value);
        }
    default:
        {
        index -= 8;

        if (index < this->p_01->getNumParameters())
            return this->p_01->constrainParameterValue(index, value);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->constrainParameterValue(index, value);

        return value;
        }
    }
}

void scheduleParamInit(ParameterIndex index, Index order) {
    this->paramInitIndices->push(index);
    this->paramInitOrder->push(order);
}

void processParamInitEvents() {
    this->listquicksort(
        this->paramInitOrder,
        this->paramInitIndices,
        0,
        (int)(this->paramInitOrder->length - 1),
        true
    );

    for (Index i = 0; i < this->paramInitOrder->length; i++) {
        this->getEngine()->scheduleParameterBang(this->paramInitIndices[i], 0);
    }
}

void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
    RNBO_UNUSED(hasValue);
    this->updateTime(time);

    switch (index) {
    case 1646922831:
        {
        this->snapshot_01_out_set(value);
        break;
        }
    case -1580124982:
        {
        this->linetilde_05_target_bang();
        break;
        }
    case 1920515087:
        {
        this->linetilde_06_target_bang();
        break;
        }
    case -21003509:
        {
        this->linetilde_07_target_bang();
        break;
        }
    case 1538117964:
        {
        this->linetilde_08_target_bang();
        break;
        }
    case -1494586265:
        {
        this->numbertilde_03_value_set(value);
        break;
        }
    case -1197727859:
        {
        this->linetilde_09_target_bang();
        break;
        }
    case 361393614:
        {
        this->linetilde_10_target_bang();
        break;
        }
    case -815330736:
        {
        this->linetilde_11_target_bang();
        break;
        }
    case 743790737:
        {
        this->linetilde_12_target_bang();
        break;
        }
    case 349887047:
        {
        this->linetilde_13_target_bang();
        break;
        }
    case 1909008520:
        {
        this->linetilde_14_target_bang();
        break;
        }
    case -826837303:
        {
        this->linetilde_15_target_bang();
        break;
        }
    case 732284170:
        {
        this->linetilde_16_target_bang();
        break;
        }
    case -2003561653:
        {
        this->linetilde_17_target_bang();
        break;
        }
    case -444440180:
        {
        this->linetilde_18_target_bang();
        break;
        }
    case 1114681293:
        {
        this->linetilde_19_target_bang();
        break;
        }
    case -1621164530:
        {
        this->linetilde_20_target_bang();
        break;
        }
    case -62043057:
        {
        this->linetilde_21_target_bang();
        break;
        }
    }
}

void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}

void processOutletEvent(
    EngineLink* sender,
    OutletIndex index,
    ParameterValue value,
    MillisecondTime time
) {
    this->updateTime(time);
    this->processOutletAtCurrentTime(sender, index, value);
}

void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
    this->updateTime(time);

    switch (tag) {
    case TAG("valin"):
        {
        if (TAG("number_obj-73") == objectId)
            this->numberobj_07_valin_set(payload);

        if (TAG("number_obj-86") == objectId)
            this->numberobj_08_valin_set(payload);

        if (TAG("number_obj-65") == objectId)
            this->numberobj_09_valin_set(payload);

        if (TAG("number_obj-14") == objectId)
            this->numberobj_10_valin_set(payload);

        if (TAG("number_obj-27") == objectId)
            this->numberobj_11_valin_set(payload);

        if (TAG("number_obj-99") == objectId)
            this->numberobj_12_valin_set(payload);

        if (TAG("number_obj-167") == objectId)
            this->numberobj_13_valin_set(payload);

        if (TAG("number_obj-48") == objectId)
            this->numberobj_14_valin_set(payload);

        if (TAG("number_obj-67") == objectId)
            this->numberobj_15_valin_set(payload);

        if (TAG("number_obj-22") == objectId)
            this->numberobj_16_valin_set(payload);

        if (TAG("number_obj-78") == objectId)
            this->numberobj_17_valin_set(payload);

        if (TAG("number_obj-102") == objectId)
            this->numberobj_18_valin_set(payload);

        if (TAG("number_obj-125") == objectId)
            this->numberobj_19_valin_set(payload);

        if (TAG("number_obj-129") == objectId)
            this->numberobj_20_valin_set(payload);

        if (TAG("number_obj-134") == objectId)
            this->numberobj_21_valin_set(payload);

        if (TAG("number_obj-136") == objectId)
            this->numberobj_22_valin_set(payload);

        if (TAG("number_obj-138") == objectId)
            this->numberobj_23_valin_set(payload);

        if (TAG("number_obj-141") == objectId)
            this->numberobj_24_valin_set(payload);

        if (TAG("number_obj-149") == objectId)
            this->numberobj_25_valin_set(payload);

        if (TAG("number_obj-155") == objectId)
            this->numberobj_26_valin_set(payload);

        if (TAG("number_obj-172") == objectId)
            this->numberobj_27_valin_set(payload);

        if (TAG("number_obj-158") == objectId)
            this->numberobj_28_valin_set(payload);

        if (TAG("number_obj-179") == objectId)
            this->numberobj_29_valin_set(payload);

        break;
        }
    case TAG("format"):
        {
        if (TAG("number_obj-73") == objectId)
            this->numberobj_07_format_set(payload);

        if (TAG("number_obj-86") == objectId)
            this->numberobj_08_format_set(payload);

        if (TAG("number_obj-65") == objectId)
            this->numberobj_09_format_set(payload);

        if (TAG("number_obj-14") == objectId)
            this->numberobj_10_format_set(payload);

        if (TAG("number_obj-27") == objectId)
            this->numberobj_11_format_set(payload);

        if (TAG("number_obj-99") == objectId)
            this->numberobj_12_format_set(payload);

        if (TAG("number_obj-167") == objectId)
            this->numberobj_13_format_set(payload);

        if (TAG("number_obj-48") == objectId)
            this->numberobj_14_format_set(payload);

        if (TAG("number_obj-67") == objectId)
            this->numberobj_15_format_set(payload);

        if (TAG("number_obj-22") == objectId)
            this->numberobj_16_format_set(payload);

        if (TAG("number_obj-78") == objectId)
            this->numberobj_17_format_set(payload);

        if (TAG("number_obj-102") == objectId)
            this->numberobj_18_format_set(payload);

        if (TAG("number_obj-125") == objectId)
            this->numberobj_19_format_set(payload);

        if (TAG("number_obj-129") == objectId)
            this->numberobj_20_format_set(payload);

        if (TAG("number_obj-134") == objectId)
            this->numberobj_21_format_set(payload);

        if (TAG("number_obj-136") == objectId)
            this->numberobj_22_format_set(payload);

        if (TAG("number_obj-138") == objectId)
            this->numberobj_23_format_set(payload);

        if (TAG("number_obj-141") == objectId)
            this->numberobj_24_format_set(payload);

        if (TAG("number_obj-149") == objectId)
            this->numberobj_25_format_set(payload);

        if (TAG("number_obj-155") == objectId)
            this->numberobj_26_format_set(payload);

        if (TAG("number_obj-172") == objectId)
            this->numberobj_27_format_set(payload);

        if (TAG("number_obj-158") == objectId)
            this->numberobj_28_format_set(payload);

        if (TAG("number_obj-179") == objectId)
            this->numberobj_29_format_set(payload);

        break;
        }
    case TAG("sig"):
        {
        if (TAG("number~_obj-95") == objectId)
            this->numbertilde_03_sig_number_set(payload);

        break;
        }
    case TAG("mode"):
        {
        if (TAG("number~_obj-95") == objectId)
            this->numbertilde_03_mode_set(payload);

        break;
        }
    case TAG("postrender"):
        {
        this->inport_01_value_number_set(payload);
        break;
        }
    }

    this->p_01->processNumMessage(tag, objectId, time, payload);
    this->p_02->processNumMessage(tag, objectId, time, payload);
}

void processListMessage(
    MessageTag tag,
    MessageTag objectId,
    MillisecondTime time,
    const list& payload
) {
    this->updateTime(time);

    switch (tag) {
    case TAG("sig"):
        {
        if (TAG("number~_obj-95") == objectId)
            this->numbertilde_03_sig_list_set(payload);

        break;
        }
    case TAG("postrender"):
        {
        this->inport_01_value_list_set(payload);
        break;
        }
    }

    this->p_01->processListMessage(tag, objectId, time, payload);
    this->p_02->processListMessage(tag, objectId, time, payload);
}

void processBangMessage(MessageTag tag, MessageTag objectId, MillisecondTime time) {
    this->updateTime(time);

    switch (tag) {
    case TAG("bangin"):
        {
        if (TAG("button_obj-20") == objectId)
            this->button_01_bangin_bang();

        break;
        }
    case TAG("postrender"):
        {
        this->inport_01_value_bang_bang();
        break;
        }
    }

    this->p_01->processBangMessage(tag, objectId, time);
    this->p_02->processBangMessage(tag, objectId, time);
}

MessageTagInfo resolveTag(MessageTag tag) const {
    switch (tag) {
    case TAG("valout"):
        {
        return "valout";
        }
    case TAG("number_obj-73"):
        {
        return "number_obj-73";
        }
    case TAG("setup"):
        {
        return "setup";
        }
    case TAG("number_obj-86"):
        {
        return "number_obj-86";
        }
    case TAG("number_obj-65"):
        {
        return "number_obj-65";
        }
    case TAG("monitor"):
        {
        return "monitor";
        }
    case TAG("number~_obj-95"):
        {
        return "number~_obj-95";
        }
    case TAG("assign"):
        {
        return "assign";
        }
    case TAG("number_obj-14"):
        {
        return "number_obj-14";
        }
    case TAG("mod"):
        {
        return "mod";
        }
    case TAG(""):
        {
        return "";
        }
    case TAG("number_obj-27"):
        {
        return "number_obj-27";
        }
    case TAG("number_obj-99"):
        {
        return "number_obj-99";
        }
    case TAG("number_obj-167"):
        {
        return "number_obj-167";
        }
    case TAG("bangout"):
        {
        return "bangout";
        }
    case TAG("button_obj-20"):
        {
        return "button_obj-20";
        }
    case TAG("number_obj-48"):
        {
        return "number_obj-48";
        }
    case TAG("number_obj-67"):
        {
        return "number_obj-67";
        }
    case TAG("number_obj-22"):
        {
        return "number_obj-22";
        }
    case TAG("number_obj-78"):
        {
        return "number_obj-78";
        }
    case TAG("number_obj-102"):
        {
        return "number_obj-102";
        }
    case TAG("number_obj-125"):
        {
        return "number_obj-125";
        }
    case TAG("number_obj-129"):
        {
        return "number_obj-129";
        }
    case TAG("number_obj-134"):
        {
        return "number_obj-134";
        }
    case TAG("number_obj-136"):
        {
        return "number_obj-136";
        }
    case TAG("number_obj-138"):
        {
        return "number_obj-138";
        }
    case TAG("number_obj-141"):
        {
        return "number_obj-141";
        }
    case TAG("number_obj-149"):
        {
        return "number_obj-149";
        }
    case TAG("number_obj-155"):
        {
        return "number_obj-155";
        }
    case TAG("number_obj-172"):
        {
        return "number_obj-172";
        }
    case TAG("number_obj-158"):
        {
        return "number_obj-158";
        }
    case TAG("number_obj-179"):
        {
        return "number_obj-179";
        }
    case TAG("valin"):
        {
        return "valin";
        }
    case TAG("format"):
        {
        return "format";
        }
    case TAG("sig"):
        {
        return "sig";
        }
    case TAG("mode"):
        {
        return "mode";
        }
    case TAG("bangin"):
        {
        return "bangin";
        }
    case TAG("postrender"):
        {
        return "postrender";
        }
    }

    auto subpatchResult_0 = this->p_01->resolveTag(tag);

    if (subpatchResult_0)
        return subpatchResult_0;

    auto subpatchResult_1 = this->p_02->resolveTag(tag);

    if (subpatchResult_1)
        return subpatchResult_1;

    return "";
}

MessageIndex getNumMessages() const {
    return 2;
}

const MessageInfo& getMessageInfo(MessageIndex index) const {
    switch (index) {
    case 0:
        {
        static const MessageInfo r0 = {
            "mod",
            Outport
        };

        return r0;
        }
    case 1:
        {
        static const MessageInfo r1 = {
            "postrender",
            Inport
        };

        return r1;
        }
    }

    return NullMessageInfo;
}

protected:

void param_01_value_set(number v) {
    v = this->param_01_value_constrain(v);
    this->param_01_value = v;
    this->sendParameter(0, false);

    if (this->param_01_value != this->param_01_lastValue) {
        this->getEngine()->presetTouched();
        this->param_01_lastValue = this->param_01_value;
    }

    this->expr_07_in1_set(v);
}

void param_02_value_set(number v) {
    v = this->param_02_value_constrain(v);
    this->param_02_value = v;
    this->sendParameter(1, false);

    if (this->param_02_value != this->param_02_lastValue) {
        this->getEngine()->presetTouched();
        this->param_02_lastValue = this->param_02_value;
    }

    this->expr_09_in1_set(v);
}

void param_03_value_set(number v) {
    v = this->param_03_value_constrain(v);
    this->param_03_value = v;
    this->sendParameter(2, false);

    if (this->param_03_value != this->param_03_lastValue) {
        this->getEngine()->presetTouched();
        this->param_03_lastValue = this->param_03_value;
    }

    this->numberobj_10_value_set(v);
}

void param_04_value_set(number v) {
    v = this->param_04_value_constrain(v);
    this->param_04_value = v;
    this->sendParameter(3, false);

    if (this->param_04_value != this->param_04_lastValue) {
        this->getEngine()->presetTouched();
        this->param_04_lastValue = this->param_04_value;
    }

    {
        list converted = {v};
        this->linetilde_09_segments_set(converted);
    }
}

void param_05_value_set(number v) {
    v = this->param_05_value_constrain(v);
    this->param_05_value = v;
    this->sendParameter(4, false);

    if (this->param_05_value != this->param_05_lastValue) {
        this->getEngine()->presetTouched();
        this->param_05_lastValue = this->param_05_value;
    }

    {
        list converted = {v};
        this->linetilde_10_segments_set(converted);
    }
}

void param_06_value_set(number v) {
    v = this->param_06_value_constrain(v);
    this->param_06_value = v;
    this->sendParameter(5, false);

    if (this->param_06_value != this->param_06_lastValue) {
        this->getEngine()->presetTouched();
        this->param_06_lastValue = this->param_06_value;
    }

    this->expr_11_in1_set(v);
}

void param_07_value_set(number v) {
    v = this->param_07_value_constrain(v);
    this->param_07_value = v;
    this->sendParameter(6, false);

    if (this->param_07_value != this->param_07_lastValue) {
        this->getEngine()->presetTouched();
        this->param_07_lastValue = this->param_07_value;
    }

    this->numberobj_13_value_set(v);
    this->expr_12_in1_set(v);
}

void param_08_value_set(number v) {
    v = this->param_08_value_constrain(v);
    this->param_08_value = v;
    this->sendParameter(7, false);

    if (this->param_08_value != this->param_08_lastValue) {
        this->getEngine()->presetTouched();
        this->param_08_lastValue = this->param_08_value;
    }

    this->numberobj_16_value_set(v);
    this->codebox_01_in1_set(v);
}

void numberobj_07_valin_set(number v) {
    this->numberobj_07_value_set(v);
}

void numberobj_07_format_set(number v) {
    this->numberobj_07_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_08_valin_set(number v) {
    this->numberobj_08_value_set(v);
}

void numberobj_08_format_set(number v) {
    this->numberobj_08_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_09_valin_set(number v) {
    this->numberobj_09_value_set(v);
}

void numberobj_09_format_set(number v) {
    this->numberobj_09_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numbertilde_03_sig_number_set(number v) {
    this->numbertilde_03_outValue = v;
}

void numbertilde_03_sig_list_set(const list& v) {
    this->numbertilde_03_outValue = v[0];
}

void numbertilde_03_mode_set(number v) {
    if (v == 1) {
        this->numbertilde_03_currentMode = 0;
    } else if (v == 2) {
        this->numbertilde_03_currentMode = 1;
    }
}

void numberobj_10_valin_set(number v) {
    this->numberobj_10_value_set(v);
}

void numberobj_10_format_set(number v) {
    this->numberobj_10_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void snapshot_01_out_set(number v) {
    this->snapshot_01_out = v;

    {
        list converted = {v};
        this->scale_01_input_set(converted);
    }
}

void numberobj_11_valin_set(number v) {
    this->numberobj_11_value_set(v);
}

void numberobj_11_format_set(number v) {
    this->numberobj_11_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_12_valin_set(number v) {
    this->numberobj_12_value_set(v);
}

void numberobj_12_format_set(number v) {
    this->numberobj_12_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_13_valin_set(number v) {
    this->numberobj_13_value_set(v);
}

void numberobj_13_format_set(number v) {
    this->numberobj_13_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void button_01_bangin_bang() {
    this->button_01_bangval_bang();
}

void inport_01_value_bang_bang() {
    this->inport_01_out_bang_bang();
}

void inport_01_value_number_set(number v) {
    this->inport_01_out_number_set(v);
}

void inport_01_value_list_set(const list& v) {
    this->inport_01_out_list_set(v);
}

void numberobj_14_valin_set(number v) {
    this->numberobj_14_value_set(v);
}

void numberobj_14_format_set(number v) {
    this->numberobj_14_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_15_valin_set(number v) {
    this->numberobj_15_value_set(v);
}

void numberobj_15_format_set(number v) {
    this->numberobj_15_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_16_valin_set(number v) {
    this->numberobj_16_value_set(v);
}

void numberobj_16_format_set(number v) {
    this->numberobj_16_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_17_valin_set(number v) {
    this->numberobj_17_value_set(v);
}

void numberobj_17_format_set(number v) {
    this->numberobj_17_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_18_valin_set(number v) {
    this->numberobj_18_value_set(v);
}

void numberobj_18_format_set(number v) {
    this->numberobj_18_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_19_valin_set(number v) {
    this->numberobj_19_value_set(v);
}

void numberobj_19_format_set(number v) {
    this->numberobj_19_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_20_valin_set(number v) {
    this->numberobj_20_value_set(v);
}

void numberobj_20_format_set(number v) {
    this->numberobj_20_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_21_valin_set(number v) {
    this->numberobj_21_value_set(v);
}

void numberobj_21_format_set(number v) {
    this->numberobj_21_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_22_valin_set(number v) {
    this->numberobj_22_value_set(v);
}

void numberobj_22_format_set(number v) {
    this->numberobj_22_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_23_valin_set(number v) {
    this->numberobj_23_value_set(v);
}

void numberobj_23_format_set(number v) {
    this->numberobj_23_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_24_valin_set(number v) {
    this->numberobj_24_value_set(v);
}

void numberobj_24_format_set(number v) {
    this->numberobj_24_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_25_valin_set(number v) {
    this->numberobj_25_value_set(v);
}

void numberobj_25_format_set(number v) {
    this->numberobj_25_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_26_valin_set(number v) {
    this->numberobj_26_value_set(v);
}

void numberobj_26_format_set(number v) {
    this->numberobj_26_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_27_valin_set(number v) {
    this->numberobj_27_value_set(v);
}

void numberobj_27_format_set(number v) {
    this->numberobj_27_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_28_valin_set(number v) {
    this->numberobj_28_value_set(v);
}

void numberobj_28_format_set(number v) {
    this->numberobj_28_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_29_valin_set(number v) {
    this->numberobj_29_value_set(v);
}

void numberobj_29_format_set(number v) {
    this->numberobj_29_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void linetilde_05_target_bang() {}

void linetilde_06_target_bang() {}

void linetilde_07_target_bang() {}

void linetilde_08_target_bang() {}

void numbertilde_03_value_set(number ) {}

void linetilde_09_target_bang() {}

void linetilde_10_target_bang() {}

void linetilde_11_target_bang() {}

void linetilde_12_target_bang() {}

void linetilde_13_target_bang() {}

void linetilde_14_target_bang() {}

void linetilde_15_target_bang() {}

void linetilde_16_target_bang() {}

void linetilde_17_target_bang() {}

void linetilde_18_target_bang() {}

void linetilde_19_target_bang() {}

void linetilde_20_target_bang() {}

void linetilde_21_target_bang() {}

number msToSamps(MillisecondTime ms, number sampleRate) {
    return ms * sampleRate * 0.001;
}

MillisecondTime sampsToMs(SampleIndex samps) {
    return samps * (this->invsr * 1000);
}

Index getMaxBlockSize() const {
    return this->maxvs;
}

number getSampleRate() const {
    return this->sr;
}

bool hasFixedVectorSize() const {
    return false;
}

Index getNumInputChannels() const {
    return 2;
}

Index getNumOutputChannels() const {
    return 2;
}

void allocateDataRefs() {
    this->p_01->allocateDataRefs();
    this->p_02->allocateDataRefs();
}

void initializeObjects() {
    this->gen_01_hb_lp_l_init();
    this->gen_01_hb_lp2_l_init();
    this->gen_01_hb_lp_r_init();
    this->gen_01_hb_lp2_r_init();
    this->gen_01_press_low_l_init();
    this->gen_01_press_high_l_init();
    this->gen_01_press_low_r_init();
    this->gen_01_press_high_r_init();
    this->gen_02_ub_lp1_l_init();
    this->gen_02_ub_lp2_l_init();
    this->gen_02_ub_lp1_r_init();
    this->gen_02_ub_lp2_r_init();
    this->gen_03_x1_l_init();
    this->gen_03_y1_l_init();
    this->gen_03_x1_r_init();
    this->gen_03_y1_r_init();
    this->gen_03_lp_l_init();
    this->gen_03_lp_r_init();
    this->numberobj_07_init();
    this->numberobj_08_init();
    this->numberobj_09_init();
    this->numbertilde_03_init();
    this->numberobj_10_init();
    this->ip_01_init();
    this->numberobj_11_init();
    this->numberobj_12_init();
    this->numberobj_13_init();
    this->numberobj_14_init();
    this->numberobj_15_init();
    this->numberobj_16_init();
    this->numberobj_17_init();
    this->numberobj_18_init();
    this->numberobj_19_init();
    this->numberobj_20_init();
    this->numberobj_21_init();
    this->numberobj_22_init();
    this->numberobj_23_init();
    this->numberobj_24_init();
    this->numberobj_25_init();
    this->numberobj_26_init();
    this->numberobj_27_init();
    this->numberobj_28_init();
    this->numberobj_29_init();
    this->p_01->initializeObjects();
    this->p_02->initializeObjects();
}

void sendOutlet(OutletIndex index, ParameterValue value) {
    this->getEngine()->sendOutlet(this, index, value);
}

void startup() {
    this->updateTime(this->getEngine()->getCurrentTime());
    this->p_01->startup();
    this->p_02->startup();

    {
        this->scheduleParamInit(0, 0);
    }

    {
        this->scheduleParamInit(1, 0);
    }

    {
        this->scheduleParamInit(2, 0);
    }

    {
        this->scheduleParamInit(3, 0);
    }

    {
        this->scheduleParamInit(4, 0);
    }

    {
        this->scheduleParamInit(5, 0);
    }

    {
        this->scheduleParamInit(6, 0);
    }

    {
        this->scheduleParamInit(7, 0);
    }

    this->processParamInitEvents();
}

number param_01_value_constrain(number v) const {
    v = (v > 24 ? 24 : (v < -24 ? -24 : v));
    return v;
}

void linetilde_06_time_set(number v) {
    this->linetilde_06_time = v;
}

void linetilde_06_segments_set(const list& v) {
    this->linetilde_06_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        if (v->length == 1 && this->linetilde_06_time == 0) {
            this->linetilde_06_activeRamps->length = 0;
            this->linetilde_06_currentValue = v[0];
        } else {
            auto currentTime = this->currentsampletime();
            number lastRampValue = this->linetilde_06_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

            for (Index i = 0; i < this->linetilde_06_activeRamps->length; i += 3) {
                rampEnd = this->linetilde_06_activeRamps[(Index)(i + 2)];

                if (rampEnd > currentTime) {
                    this->linetilde_06_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->linetilde_06_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->linetilde_06_activeRamps[(Index)i] - valueDiff;
                    this->linetilde_06_activeRamps[(Index)i] = lastRampValue;
                    this->linetilde_06_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->linetilde_06_activeRamps[(Index)i];
                }
            }

            if (rampEnd < currentTime) {
                this->linetilde_06_activeRamps->push(lastRampValue);
                this->linetilde_06_activeRamps->push(0);
                this->linetilde_06_activeRamps->push(currentTime);
            }

            number lastRampEnd = currentTime;

            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;

                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);

                    if ((bool)(this->linetilde_06_keepramp)) {
                        this->linetilde_06_time_set(v[(Index)(i + 1)]);
                    }
                } else {
                    rampTimeInSamples = this->mstosamps(this->linetilde_06_time);
                }

                if (rampTimeInSamples <= 0) {
                    rampTimeInSamples = 1;
                }

                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->linetilde_06_activeRamps->push(destinationValue);
                this->linetilde_06_activeRamps->push(inc);
                this->linetilde_06_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
}

void expr_07_out1_set(number v) {
    this->expr_07_out1 = v;

    {
        list converted = {this->expr_07_out1};
        this->linetilde_06_segments_set(converted);
    }
}

void expr_07_in1_set(number in1) {
    this->expr_07_in1 = in1;
    this->expr_07_out1_set(rnbo_pow(10, this->expr_07_in1 * 0.05));//#map:dbtoa_obj-26:1
}

number param_02_value_constrain(number v) const {
    v = (v > 24 ? 24 : (v < -24 ? -24 : v));
    return v;
}

void linetilde_08_time_set(number v) {
    this->linetilde_08_time = v;
}

void linetilde_08_segments_set(const list& v) {
    this->linetilde_08_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        if (v->length == 1 && this->linetilde_08_time == 0) {
            this->linetilde_08_activeRamps->length = 0;
            this->linetilde_08_currentValue = v[0];
        } else {
            auto currentTime = this->currentsampletime();
            number lastRampValue = this->linetilde_08_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

            for (Index i = 0; i < this->linetilde_08_activeRamps->length; i += 3) {
                rampEnd = this->linetilde_08_activeRamps[(Index)(i + 2)];

                if (rampEnd > currentTime) {
                    this->linetilde_08_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->linetilde_08_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->linetilde_08_activeRamps[(Index)i] - valueDiff;
                    this->linetilde_08_activeRamps[(Index)i] = lastRampValue;
                    this->linetilde_08_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->linetilde_08_activeRamps[(Index)i];
                }
            }

            if (rampEnd < currentTime) {
                this->linetilde_08_activeRamps->push(lastRampValue);
                this->linetilde_08_activeRamps->push(0);
                this->linetilde_08_activeRamps->push(currentTime);
            }

            number lastRampEnd = currentTime;

            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;

                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);

                    if ((bool)(this->linetilde_08_keepramp)) {
                        this->linetilde_08_time_set(v[(Index)(i + 1)]);
                    }
                } else {
                    rampTimeInSamples = this->mstosamps(this->linetilde_08_time);
                }

                if (rampTimeInSamples <= 0) {
                    rampTimeInSamples = 1;
                }

                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->linetilde_08_activeRamps->push(destinationValue);
                this->linetilde_08_activeRamps->push(inc);
                this->linetilde_08_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
}

void expr_09_out1_set(number v) {
    this->expr_09_out1 = v;

    {
        list converted = {this->expr_09_out1};
        this->linetilde_08_segments_set(converted);
    }
}

void expr_09_in1_set(number in1) {
    this->expr_09_in1 = in1;
    this->expr_09_out1_set(rnbo_pow(10, this->expr_09_in1 * 0.05));//#map:dbtoa_obj-96:1
}

number param_03_value_constrain(number v) const {
    v = (v > 2 ? 2 : (v < 1 ? 1 : v));
    return v;
}

void numberobj_10_output_set(number ) {}

void numberobj_10_value_set(number v) {
    this->numberobj_10_value_setter(v);
    v = this->numberobj_10_value;
    number localvalue = v;

    if (this->numberobj_10_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-14"), localvalue, this->_currentTime);
    this->numberobj_10_output_set(localvalue);
}

number param_04_value_constrain(number v) const {
    v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    return v;
}

void linetilde_09_time_set(number v) {
    this->linetilde_09_time = v;
}

void linetilde_09_segments_set(const list& v) {
    this->linetilde_09_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        if (v->length == 1 && this->linetilde_09_time == 0) {
            this->linetilde_09_activeRamps->length = 0;
            this->linetilde_09_currentValue = v[0];
        } else {
            auto currentTime = this->currentsampletime();
            number lastRampValue = this->linetilde_09_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

            for (Index i = 0; i < this->linetilde_09_activeRamps->length; i += 3) {
                rampEnd = this->linetilde_09_activeRamps[(Index)(i + 2)];

                if (rampEnd > currentTime) {
                    this->linetilde_09_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->linetilde_09_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->linetilde_09_activeRamps[(Index)i] - valueDiff;
                    this->linetilde_09_activeRamps[(Index)i] = lastRampValue;
                    this->linetilde_09_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->linetilde_09_activeRamps[(Index)i];
                }
            }

            if (rampEnd < currentTime) {
                this->linetilde_09_activeRamps->push(lastRampValue);
                this->linetilde_09_activeRamps->push(0);
                this->linetilde_09_activeRamps->push(currentTime);
            }

            number lastRampEnd = currentTime;

            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;

                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);

                    if ((bool)(this->linetilde_09_keepramp)) {
                        this->linetilde_09_time_set(v[(Index)(i + 1)]);
                    }
                } else {
                    rampTimeInSamples = this->mstosamps(this->linetilde_09_time);
                }

                if (rampTimeInSamples <= 0) {
                    rampTimeInSamples = 1;
                }

                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->linetilde_09_activeRamps->push(destinationValue);
                this->linetilde_09_activeRamps->push(inc);
                this->linetilde_09_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
}

number param_05_value_constrain(number v) const {
    v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    return v;
}

void linetilde_10_time_set(number v) {
    this->linetilde_10_time = v;
}

void linetilde_10_segments_set(const list& v) {
    this->linetilde_10_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        if (v->length == 1 && this->linetilde_10_time == 0) {
            this->linetilde_10_activeRamps->length = 0;
            this->linetilde_10_currentValue = v[0];
        } else {
            auto currentTime = this->currentsampletime();
            number lastRampValue = this->linetilde_10_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

            for (Index i = 0; i < this->linetilde_10_activeRamps->length; i += 3) {
                rampEnd = this->linetilde_10_activeRamps[(Index)(i + 2)];

                if (rampEnd > currentTime) {
                    this->linetilde_10_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->linetilde_10_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->linetilde_10_activeRamps[(Index)i] - valueDiff;
                    this->linetilde_10_activeRamps[(Index)i] = lastRampValue;
                    this->linetilde_10_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->linetilde_10_activeRamps[(Index)i];
                }
            }

            if (rampEnd < currentTime) {
                this->linetilde_10_activeRamps->push(lastRampValue);
                this->linetilde_10_activeRamps->push(0);
                this->linetilde_10_activeRamps->push(currentTime);
            }

            number lastRampEnd = currentTime;

            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;

                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);

                    if ((bool)(this->linetilde_10_keepramp)) {
                        this->linetilde_10_time_set(v[(Index)(i + 1)]);
                    }
                } else {
                    rampTimeInSamples = this->mstosamps(this->linetilde_10_time);
                }

                if (rampTimeInSamples <= 0) {
                    rampTimeInSamples = 1;
                }

                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->linetilde_10_activeRamps->push(destinationValue);
                this->linetilde_10_activeRamps->push(inc);
                this->linetilde_10_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
}

number param_06_value_constrain(number v) const {
    v = (v > 12 ? 12 : (v < -12 ? -12 : v));
    return v;
}

void p_02_in3_number_set(number v) {
    this->p_02->updateTime(this->_currentTime);
    this->p_02->eventinlet_02_out1_number_set(v);
}

void expr_11_out1_set(number v) {
    this->expr_11_out1 = v;
    this->p_02_in3_number_set(this->expr_11_out1);
}

void expr_11_in1_set(number in1) {
    this->expr_11_in1 = in1;
    this->expr_11_out1_set(this->expr_11_in1 + this->expr_11_in2);//#map:+_obj-177:1
}

number param_07_value_constrain(number v) const {
    v = (v > 12 ? 12 : (v < -12 ? -12 : v));
    return v;
}

void numberobj_13_output_set(number ) {}

void numberobj_13_value_set(number v) {
    this->numberobj_13_value_setter(v);
    v = this->numberobj_13_value;
    number localvalue = v;

    if (this->numberobj_13_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-167"), localvalue, this->_currentTime);
    this->numberobj_13_output_set(localvalue);
}

void numberobj_12_output_set(number ) {}

void numberobj_12_value_set(number v) {
    this->numberobj_12_value_setter(v);
    v = this->numberobj_12_value;
    number localvalue = v;

    if (this->numberobj_12_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-99"), localvalue, this->_currentTime);
    this->numberobj_12_output_set(localvalue);
}

void p_01_in3_number_set(number v) {
    this->p_01->updateTime(this->_currentTime);
    this->p_01->eventinlet_01_out1_number_set(v);
}

void expr_12_out1_set(number v) {
    this->expr_12_out1 = v;
    this->numberobj_12_value_set(this->expr_12_out1);
    this->p_01_in3_number_set(this->expr_12_out1);
}

void expr_12_in1_set(number in1) {
    this->expr_12_in1 = in1;
    this->expr_12_out1_set(this->expr_12_in1 + this->expr_12_in2);//#map:+_obj-79:1
}

number param_08_value_constrain(number v) const {
    v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    return v;
}

void numberobj_16_output_set(number ) {}

void numberobj_16_value_set(number v) {
    this->numberobj_16_value_setter(v);
    v = this->numberobj_16_value;
    number localvalue = v;

    if (this->numberobj_16_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-22"), localvalue, this->_currentTime);
    this->numberobj_16_output_set(localvalue);
}

void linetilde_11_time_set(number v) {
    this->linetilde_11_time = v;
}

void linetilde_11_segments_set(const list& v) {
    this->linetilde_11_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        if (v->length == 1 && this->linetilde_11_time == 0) {
            this->linetilde_11_activeRamps->length = 0;
            this->linetilde_11_currentValue = v[0];
        } else {
            auto currentTime = this->currentsampletime();
            number lastRampValue = this->linetilde_11_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

            for (Index i = 0; i < this->linetilde_11_activeRamps->length; i += 3) {
                rampEnd = this->linetilde_11_activeRamps[(Index)(i + 2)];

                if (rampEnd > currentTime) {
                    this->linetilde_11_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->linetilde_11_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->linetilde_11_activeRamps[(Index)i] - valueDiff;
                    this->linetilde_11_activeRamps[(Index)i] = lastRampValue;
                    this->linetilde_11_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->linetilde_11_activeRamps[(Index)i];
                }
            }

            if (rampEnd < currentTime) {
                this->linetilde_11_activeRamps->push(lastRampValue);
                this->linetilde_11_activeRamps->push(0);
                this->linetilde_11_activeRamps->push(currentTime);
            }

            number lastRampEnd = currentTime;

            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;

                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);

                    if ((bool)(this->linetilde_11_keepramp)) {
                        this->linetilde_11_time_set(v[(Index)(i + 1)]);
                    }
                } else {
                    rampTimeInSamples = this->mstosamps(this->linetilde_11_time);
                }

                if (rampTimeInSamples <= 0) {
                    rampTimeInSamples = 1;
                }

                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->linetilde_11_activeRamps->push(destinationValue);
                this->linetilde_11_activeRamps->push(inc);
                this->linetilde_11_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
}

void numberobj_14_output_set(number v) {
    {
        list converted = {v};
        this->linetilde_11_segments_set(converted);
    }
}

void numberobj_14_value_set(number v) {
    this->numberobj_14_value_setter(v);
    v = this->numberobj_14_value;
    number localvalue = v;

    if (this->numberobj_14_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-48"), localvalue, this->_currentTime);
    this->numberobj_14_output_set(localvalue);
}

void codebox_01_out1_set(number v) {
    this->codebox_01_out1 = v;
    this->numberobj_14_value_set(this->codebox_01_out1);
}

void linetilde_12_time_set(number v) {
    this->linetilde_12_time = v;
}

void linetilde_12_segments_set(const list& v) {
    this->linetilde_12_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        if (v->length == 1 && this->linetilde_12_time == 0) {
            this->linetilde_12_activeRamps->length = 0;
            this->linetilde_12_currentValue = v[0];
        } else {
            auto currentTime = this->currentsampletime();
            number lastRampValue = this->linetilde_12_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

            for (Index i = 0; i < this->linetilde_12_activeRamps->length; i += 3) {
                rampEnd = this->linetilde_12_activeRamps[(Index)(i + 2)];

                if (rampEnd > currentTime) {
                    this->linetilde_12_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->linetilde_12_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->linetilde_12_activeRamps[(Index)i] - valueDiff;
                    this->linetilde_12_activeRamps[(Index)i] = lastRampValue;
                    this->linetilde_12_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->linetilde_12_activeRamps[(Index)i];
                }
            }

            if (rampEnd < currentTime) {
                this->linetilde_12_activeRamps->push(lastRampValue);
                this->linetilde_12_activeRamps->push(0);
                this->linetilde_12_activeRamps->push(currentTime);
            }

            number lastRampEnd = currentTime;

            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;

                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);

                    if ((bool)(this->linetilde_12_keepramp)) {
                        this->linetilde_12_time_set(v[(Index)(i + 1)]);
                    }
                } else {
                    rampTimeInSamples = this->mstosamps(this->linetilde_12_time);
                }

                if (rampTimeInSamples <= 0) {
                    rampTimeInSamples = 1;
                }

                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->linetilde_12_activeRamps->push(destinationValue);
                this->linetilde_12_activeRamps->push(inc);
                this->linetilde_12_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
}

void numberobj_15_output_set(number v) {
    {
        list converted = {v};
        this->linetilde_12_segments_set(converted);
    }
}

void numberobj_15_value_set(number v) {
    this->numberobj_15_value_setter(v);
    v = this->numberobj_15_value;
    number localvalue = v;

    if (this->numberobj_15_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-67"), localvalue, this->_currentTime);
    this->numberobj_15_output_set(localvalue);
}

void codebox_01_out2_set(number v) {
    this->codebox_01_out2 = v;
    this->numberobj_15_value_set(this->codebox_01_out2);
}

void linetilde_13_time_set(number v) {
    this->linetilde_13_time = v;
}

void linetilde_13_segments_set(const list& v) {
    this->linetilde_13_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        if (v->length == 1 && this->linetilde_13_time == 0) {
            this->linetilde_13_activeRamps->length = 0;
            this->linetilde_13_currentValue = v[0];
        } else {
            auto currentTime = this->currentsampletime();
            number lastRampValue = this->linetilde_13_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

            for (Index i = 0; i < this->linetilde_13_activeRamps->length; i += 3) {
                rampEnd = this->linetilde_13_activeRamps[(Index)(i + 2)];

                if (rampEnd > currentTime) {
                    this->linetilde_13_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->linetilde_13_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->linetilde_13_activeRamps[(Index)i] - valueDiff;
                    this->linetilde_13_activeRamps[(Index)i] = lastRampValue;
                    this->linetilde_13_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->linetilde_13_activeRamps[(Index)i];
                }
            }

            if (rampEnd < currentTime) {
                this->linetilde_13_activeRamps->push(lastRampValue);
                this->linetilde_13_activeRamps->push(0);
                this->linetilde_13_activeRamps->push(currentTime);
            }

            number lastRampEnd = currentTime;

            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;

                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);

                    if ((bool)(this->linetilde_13_keepramp)) {
                        this->linetilde_13_time_set(v[(Index)(i + 1)]);
                    }
                } else {
                    rampTimeInSamples = this->mstosamps(this->linetilde_13_time);
                }

                if (rampTimeInSamples <= 0) {
                    rampTimeInSamples = 1;
                }

                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->linetilde_13_activeRamps->push(destinationValue);
                this->linetilde_13_activeRamps->push(inc);
                this->linetilde_13_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
}

void numberobj_17_output_set(number v) {
    {
        list converted = {v};
        this->linetilde_13_segments_set(converted);
    }
}

void numberobj_17_value_set(number v) {
    this->numberobj_17_value_setter(v);
    v = this->numberobj_17_value;
    number localvalue = v;

    if (this->numberobj_17_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-78"), localvalue, this->_currentTime);
    this->numberobj_17_output_set(localvalue);
}

void codebox_01_out3_set(number v) {
    this->codebox_01_out3 = v;
    this->numberobj_17_value_set(this->codebox_01_out3);
}

void linetilde_14_time_set(number v) {
    this->linetilde_14_time = v;
}

void linetilde_14_segments_set(const list& v) {
    this->linetilde_14_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        if (v->length == 1 && this->linetilde_14_time == 0) {
            this->linetilde_14_activeRamps->length = 0;
            this->linetilde_14_currentValue = v[0];
        } else {
            auto currentTime = this->currentsampletime();
            number lastRampValue = this->linetilde_14_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

            for (Index i = 0; i < this->linetilde_14_activeRamps->length; i += 3) {
                rampEnd = this->linetilde_14_activeRamps[(Index)(i + 2)];

                if (rampEnd > currentTime) {
                    this->linetilde_14_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->linetilde_14_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->linetilde_14_activeRamps[(Index)i] - valueDiff;
                    this->linetilde_14_activeRamps[(Index)i] = lastRampValue;
                    this->linetilde_14_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->linetilde_14_activeRamps[(Index)i];
                }
            }

            if (rampEnd < currentTime) {
                this->linetilde_14_activeRamps->push(lastRampValue);
                this->linetilde_14_activeRamps->push(0);
                this->linetilde_14_activeRamps->push(currentTime);
            }

            number lastRampEnd = currentTime;

            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;

                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);

                    if ((bool)(this->linetilde_14_keepramp)) {
                        this->linetilde_14_time_set(v[(Index)(i + 1)]);
                    }
                } else {
                    rampTimeInSamples = this->mstosamps(this->linetilde_14_time);
                }

                if (rampTimeInSamples <= 0) {
                    rampTimeInSamples = 1;
                }

                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->linetilde_14_activeRamps->push(destinationValue);
                this->linetilde_14_activeRamps->push(inc);
                this->linetilde_14_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
}

void numberobj_18_output_set(number v) {
    {
        list converted = {v};
        this->linetilde_14_segments_set(converted);
    }
}

void numberobj_18_value_set(number v) {
    this->numberobj_18_value_setter(v);
    v = this->numberobj_18_value;
    number localvalue = v;

    if (this->numberobj_18_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-102"), localvalue, this->_currentTime);
    this->numberobj_18_output_set(localvalue);
}

void codebox_01_out4_set(number v) {
    this->codebox_01_out4 = v;
    this->numberobj_18_value_set(this->codebox_01_out4);
}

void linetilde_15_time_set(number v) {
    this->linetilde_15_time = v;
}

void linetilde_15_segments_set(const list& v) {
    this->linetilde_15_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        if (v->length == 1 && this->linetilde_15_time == 0) {
            this->linetilde_15_activeRamps->length = 0;
            this->linetilde_15_currentValue = v[0];
        } else {
            auto currentTime = this->currentsampletime();
            number lastRampValue = this->linetilde_15_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

            for (Index i = 0; i < this->linetilde_15_activeRamps->length; i += 3) {
                rampEnd = this->linetilde_15_activeRamps[(Index)(i + 2)];

                if (rampEnd > currentTime) {
                    this->linetilde_15_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->linetilde_15_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->linetilde_15_activeRamps[(Index)i] - valueDiff;
                    this->linetilde_15_activeRamps[(Index)i] = lastRampValue;
                    this->linetilde_15_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->linetilde_15_activeRamps[(Index)i];
                }
            }

            if (rampEnd < currentTime) {
                this->linetilde_15_activeRamps->push(lastRampValue);
                this->linetilde_15_activeRamps->push(0);
                this->linetilde_15_activeRamps->push(currentTime);
            }

            number lastRampEnd = currentTime;

            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;

                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);

                    if ((bool)(this->linetilde_15_keepramp)) {
                        this->linetilde_15_time_set(v[(Index)(i + 1)]);
                    }
                } else {
                    rampTimeInSamples = this->mstosamps(this->linetilde_15_time);
                }

                if (rampTimeInSamples <= 0) {
                    rampTimeInSamples = 1;
                }

                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->linetilde_15_activeRamps->push(destinationValue);
                this->linetilde_15_activeRamps->push(inc);
                this->linetilde_15_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
}

void numberobj_19_output_set(number v) {
    {
        list converted = {v};
        this->linetilde_15_segments_set(converted);
    }
}

void numberobj_19_value_set(number v) {
    this->numberobj_19_value_setter(v);
    v = this->numberobj_19_value;
    number localvalue = v;

    if (this->numberobj_19_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-125"), localvalue, this->_currentTime);
    this->numberobj_19_output_set(localvalue);
}

void codebox_01_out5_set(number v) {
    this->codebox_01_out5 = v;
    this->numberobj_19_value_set(this->codebox_01_out5);
}

void linetilde_16_time_set(number v) {
    this->linetilde_16_time = v;
}

void linetilde_16_segments_set(const list& v) {
    this->linetilde_16_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        if (v->length == 1 && this->linetilde_16_time == 0) {
            this->linetilde_16_activeRamps->length = 0;
            this->linetilde_16_currentValue = v[0];
        } else {
            auto currentTime = this->currentsampletime();
            number lastRampValue = this->linetilde_16_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

            for (Index i = 0; i < this->linetilde_16_activeRamps->length; i += 3) {
                rampEnd = this->linetilde_16_activeRamps[(Index)(i + 2)];

                if (rampEnd > currentTime) {
                    this->linetilde_16_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->linetilde_16_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->linetilde_16_activeRamps[(Index)i] - valueDiff;
                    this->linetilde_16_activeRamps[(Index)i] = lastRampValue;
                    this->linetilde_16_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->linetilde_16_activeRamps[(Index)i];
                }
            }

            if (rampEnd < currentTime) {
                this->linetilde_16_activeRamps->push(lastRampValue);
                this->linetilde_16_activeRamps->push(0);
                this->linetilde_16_activeRamps->push(currentTime);
            }

            number lastRampEnd = currentTime;

            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;

                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);

                    if ((bool)(this->linetilde_16_keepramp)) {
                        this->linetilde_16_time_set(v[(Index)(i + 1)]);
                    }
                } else {
                    rampTimeInSamples = this->mstosamps(this->linetilde_16_time);
                }

                if (rampTimeInSamples <= 0) {
                    rampTimeInSamples = 1;
                }

                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->linetilde_16_activeRamps->push(destinationValue);
                this->linetilde_16_activeRamps->push(inc);
                this->linetilde_16_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
}

void numberobj_20_output_set(number v) {
    {
        list converted = {v};
        this->linetilde_16_segments_set(converted);
    }
}

void numberobj_20_value_set(number v) {
    this->numberobj_20_value_setter(v);
    v = this->numberobj_20_value;
    number localvalue = v;

    if (this->numberobj_20_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-129"), localvalue, this->_currentTime);
    this->numberobj_20_output_set(localvalue);
}

void codebox_01_out6_set(number v) {
    this->codebox_01_out6 = v;
    this->numberobj_20_value_set(this->codebox_01_out6);
}

void linetilde_17_time_set(number v) {
    this->linetilde_17_time = v;
}

void linetilde_17_segments_set(const list& v) {
    this->linetilde_17_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        if (v->length == 1 && this->linetilde_17_time == 0) {
            this->linetilde_17_activeRamps->length = 0;
            this->linetilde_17_currentValue = v[0];
        } else {
            auto currentTime = this->currentsampletime();
            number lastRampValue = this->linetilde_17_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

            for (Index i = 0; i < this->linetilde_17_activeRamps->length; i += 3) {
                rampEnd = this->linetilde_17_activeRamps[(Index)(i + 2)];

                if (rampEnd > currentTime) {
                    this->linetilde_17_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->linetilde_17_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->linetilde_17_activeRamps[(Index)i] - valueDiff;
                    this->linetilde_17_activeRamps[(Index)i] = lastRampValue;
                    this->linetilde_17_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->linetilde_17_activeRamps[(Index)i];
                }
            }

            if (rampEnd < currentTime) {
                this->linetilde_17_activeRamps->push(lastRampValue);
                this->linetilde_17_activeRamps->push(0);
                this->linetilde_17_activeRamps->push(currentTime);
            }

            number lastRampEnd = currentTime;

            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;

                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);

                    if ((bool)(this->linetilde_17_keepramp)) {
                        this->linetilde_17_time_set(v[(Index)(i + 1)]);
                    }
                } else {
                    rampTimeInSamples = this->mstosamps(this->linetilde_17_time);
                }

                if (rampTimeInSamples <= 0) {
                    rampTimeInSamples = 1;
                }

                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->linetilde_17_activeRamps->push(destinationValue);
                this->linetilde_17_activeRamps->push(inc);
                this->linetilde_17_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
}

void numberobj_21_output_set(number v) {
    {
        list converted = {v};
        this->linetilde_17_segments_set(converted);
    }
}

void numberobj_21_value_set(number v) {
    this->numberobj_21_value_setter(v);
    v = this->numberobj_21_value;
    number localvalue = v;

    if (this->numberobj_21_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-134"), localvalue, this->_currentTime);
    this->numberobj_21_output_set(localvalue);
}

void codebox_01_out7_set(number v) {
    this->codebox_01_out7 = v;
    this->numberobj_21_value_set(this->codebox_01_out7);
}

void linetilde_18_time_set(number v) {
    this->linetilde_18_time = v;
}

void linetilde_18_segments_set(const list& v) {
    this->linetilde_18_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        if (v->length == 1 && this->linetilde_18_time == 0) {
            this->linetilde_18_activeRamps->length = 0;
            this->linetilde_18_currentValue = v[0];
        } else {
            auto currentTime = this->currentsampletime();
            number lastRampValue = this->linetilde_18_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

            for (Index i = 0; i < this->linetilde_18_activeRamps->length; i += 3) {
                rampEnd = this->linetilde_18_activeRamps[(Index)(i + 2)];

                if (rampEnd > currentTime) {
                    this->linetilde_18_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->linetilde_18_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->linetilde_18_activeRamps[(Index)i] - valueDiff;
                    this->linetilde_18_activeRamps[(Index)i] = lastRampValue;
                    this->linetilde_18_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->linetilde_18_activeRamps[(Index)i];
                }
            }

            if (rampEnd < currentTime) {
                this->linetilde_18_activeRamps->push(lastRampValue);
                this->linetilde_18_activeRamps->push(0);
                this->linetilde_18_activeRamps->push(currentTime);
            }

            number lastRampEnd = currentTime;

            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;

                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);

                    if ((bool)(this->linetilde_18_keepramp)) {
                        this->linetilde_18_time_set(v[(Index)(i + 1)]);
                    }
                } else {
                    rampTimeInSamples = this->mstosamps(this->linetilde_18_time);
                }

                if (rampTimeInSamples <= 0) {
                    rampTimeInSamples = 1;
                }

                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->linetilde_18_activeRamps->push(destinationValue);
                this->linetilde_18_activeRamps->push(inc);
                this->linetilde_18_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
}

void numberobj_22_output_set(number v) {
    {
        list converted = {v};
        this->linetilde_18_segments_set(converted);
    }
}

void numberobj_22_value_set(number v) {
    this->numberobj_22_value_setter(v);
    v = this->numberobj_22_value;
    number localvalue = v;

    if (this->numberobj_22_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-136"), localvalue, this->_currentTime);
    this->numberobj_22_output_set(localvalue);
}

void codebox_01_out8_set(number v) {
    this->codebox_01_out8 = v;
    this->numberobj_22_value_set(this->codebox_01_out8);
}

void linetilde_07_time_set(number v) {
    this->linetilde_07_time = v;
}

void linetilde_07_segments_set(const list& v) {
    this->linetilde_07_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        if (v->length == 1 && this->linetilde_07_time == 0) {
            this->linetilde_07_activeRamps->length = 0;
            this->linetilde_07_currentValue = v[0];
        } else {
            auto currentTime = this->currentsampletime();
            number lastRampValue = this->linetilde_07_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

            for (Index i = 0; i < this->linetilde_07_activeRamps->length; i += 3) {
                rampEnd = this->linetilde_07_activeRamps[(Index)(i + 2)];

                if (rampEnd > currentTime) {
                    this->linetilde_07_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->linetilde_07_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->linetilde_07_activeRamps[(Index)i] - valueDiff;
                    this->linetilde_07_activeRamps[(Index)i] = lastRampValue;
                    this->linetilde_07_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->linetilde_07_activeRamps[(Index)i];
                }
            }

            if (rampEnd < currentTime) {
                this->linetilde_07_activeRamps->push(lastRampValue);
                this->linetilde_07_activeRamps->push(0);
                this->linetilde_07_activeRamps->push(currentTime);
            }

            number lastRampEnd = currentTime;

            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;

                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);

                    if ((bool)(this->linetilde_07_keepramp)) {
                        this->linetilde_07_time_set(v[(Index)(i + 1)]);
                    }
                } else {
                    rampTimeInSamples = this->mstosamps(this->linetilde_07_time);
                }

                if (rampTimeInSamples <= 0) {
                    rampTimeInSamples = 1;
                }

                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->linetilde_07_activeRamps->push(destinationValue);
                this->linetilde_07_activeRamps->push(inc);
                this->linetilde_07_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
}

void expr_08_out1_set(number v) {
    this->expr_08_out1 = v;

    {
        list converted = {this->expr_08_out1};
        this->linetilde_07_segments_set(converted);
    }
}

void expr_08_in1_set(number in1) {
    this->expr_08_in1 = in1;
    this->expr_08_out1_set(rnbo_pow(10, this->expr_08_in1 * 0.05));//#map:dbtoa_obj-31:1
}

void numberobj_09_output_set(number v) {
    this->expr_08_in1_set(v);
}

void numberobj_09_value_set(number v) {
    this->numberobj_09_value_setter(v);
    v = this->numberobj_09_value;
    number localvalue = v;

    if (this->numberobj_09_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-65"), localvalue, this->_currentTime);
    this->numberobj_09_output_set(localvalue);
}

void numberobj_23_output_set(number v) {
    this->numberobj_09_value_set(v);
}

void numberobj_23_value_set(number v) {
    this->numberobj_23_value_setter(v);
    v = this->numberobj_23_value;
    number localvalue = v;

    if (this->numberobj_23_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-138"), localvalue, this->_currentTime);
    this->numberobj_23_output_set(localvalue);
}

void codebox_01_out9_set(number v) {
    this->codebox_01_out9 = v;
    this->numberobj_23_value_set(this->codebox_01_out9);
}

void linetilde_19_time_set(number v) {
    this->linetilde_19_time = v;
}

void linetilde_19_segments_set(const list& v) {
    this->linetilde_19_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        if (v->length == 1 && this->linetilde_19_time == 0) {
            this->linetilde_19_activeRamps->length = 0;
            this->linetilde_19_currentValue = v[0];
        } else {
            auto currentTime = this->currentsampletime();
            number lastRampValue = this->linetilde_19_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

            for (Index i = 0; i < this->linetilde_19_activeRamps->length; i += 3) {
                rampEnd = this->linetilde_19_activeRamps[(Index)(i + 2)];

                if (rampEnd > currentTime) {
                    this->linetilde_19_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->linetilde_19_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->linetilde_19_activeRamps[(Index)i] - valueDiff;
                    this->linetilde_19_activeRamps[(Index)i] = lastRampValue;
                    this->linetilde_19_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->linetilde_19_activeRamps[(Index)i];
                }
            }

            if (rampEnd < currentTime) {
                this->linetilde_19_activeRamps->push(lastRampValue);
                this->linetilde_19_activeRamps->push(0);
                this->linetilde_19_activeRamps->push(currentTime);
            }

            number lastRampEnd = currentTime;

            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;

                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);

                    if ((bool)(this->linetilde_19_keepramp)) {
                        this->linetilde_19_time_set(v[(Index)(i + 1)]);
                    }
                } else {
                    rampTimeInSamples = this->mstosamps(this->linetilde_19_time);
                }

                if (rampTimeInSamples <= 0) {
                    rampTimeInSamples = 1;
                }

                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->linetilde_19_activeRamps->push(destinationValue);
                this->linetilde_19_activeRamps->push(inc);
                this->linetilde_19_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
}

void numberobj_24_output_set(number v) {
    {
        list converted = {v};
        this->linetilde_19_segments_set(converted);
    }
}

void numberobj_24_value_set(number v) {
    this->numberobj_24_value_setter(v);
    v = this->numberobj_24_value;
    number localvalue = v;

    if (this->numberobj_24_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-141"), localvalue, this->_currentTime);
    this->numberobj_24_output_set(localvalue);
}

void codebox_01_out10_set(number v) {
    this->codebox_01_out10 = v;
    this->numberobj_24_value_set(this->codebox_01_out10);
}

void linetilde_20_time_set(number v) {
    this->linetilde_20_time = v;
}

void linetilde_20_segments_set(const list& v) {
    this->linetilde_20_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        if (v->length == 1 && this->linetilde_20_time == 0) {
            this->linetilde_20_activeRamps->length = 0;
            this->linetilde_20_currentValue = v[0];
        } else {
            auto currentTime = this->currentsampletime();
            number lastRampValue = this->linetilde_20_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

            for (Index i = 0; i < this->linetilde_20_activeRamps->length; i += 3) {
                rampEnd = this->linetilde_20_activeRamps[(Index)(i + 2)];

                if (rampEnd > currentTime) {
                    this->linetilde_20_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->linetilde_20_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->linetilde_20_activeRamps[(Index)i] - valueDiff;
                    this->linetilde_20_activeRamps[(Index)i] = lastRampValue;
                    this->linetilde_20_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->linetilde_20_activeRamps[(Index)i];
                }
            }

            if (rampEnd < currentTime) {
                this->linetilde_20_activeRamps->push(lastRampValue);
                this->linetilde_20_activeRamps->push(0);
                this->linetilde_20_activeRamps->push(currentTime);
            }

            number lastRampEnd = currentTime;

            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;

                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);

                    if ((bool)(this->linetilde_20_keepramp)) {
                        this->linetilde_20_time_set(v[(Index)(i + 1)]);
                    }
                } else {
                    rampTimeInSamples = this->mstosamps(this->linetilde_20_time);
                }

                if (rampTimeInSamples <= 0) {
                    rampTimeInSamples = 1;
                }

                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->linetilde_20_activeRamps->push(destinationValue);
                this->linetilde_20_activeRamps->push(inc);
                this->linetilde_20_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
}

void numberobj_25_output_set(number v) {
    {
        list converted = {v};
        this->linetilde_20_segments_set(converted);
    }
}

void numberobj_25_value_set(number v) {
    this->numberobj_25_value_setter(v);
    v = this->numberobj_25_value;
    number localvalue = v;

    if (this->numberobj_25_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-149"), localvalue, this->_currentTime);
    this->numberobj_25_output_set(localvalue);
}

void codebox_01_out11_set(number v) {
    this->codebox_01_out11 = v;
    this->numberobj_25_value_set(this->codebox_01_out11);
}

void linetilde_21_time_set(number v) {
    this->linetilde_21_time = v;
}

void linetilde_21_segments_set(const list& v) {
    this->linetilde_21_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        if (v->length == 1 && this->linetilde_21_time == 0) {
            this->linetilde_21_activeRamps->length = 0;
            this->linetilde_21_currentValue = v[0];
        } else {
            auto currentTime = this->currentsampletime();
            number lastRampValue = this->linetilde_21_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

            for (Index i = 0; i < this->linetilde_21_activeRamps->length; i += 3) {
                rampEnd = this->linetilde_21_activeRamps[(Index)(i + 2)];

                if (rampEnd > currentTime) {
                    this->linetilde_21_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->linetilde_21_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->linetilde_21_activeRamps[(Index)i] - valueDiff;
                    this->linetilde_21_activeRamps[(Index)i] = lastRampValue;
                    this->linetilde_21_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->linetilde_21_activeRamps[(Index)i];
                }
            }

            if (rampEnd < currentTime) {
                this->linetilde_21_activeRamps->push(lastRampValue);
                this->linetilde_21_activeRamps->push(0);
                this->linetilde_21_activeRamps->push(currentTime);
            }

            number lastRampEnd = currentTime;

            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;

                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);

                    if ((bool)(this->linetilde_21_keepramp)) {
                        this->linetilde_21_time_set(v[(Index)(i + 1)]);
                    }
                } else {
                    rampTimeInSamples = this->mstosamps(this->linetilde_21_time);
                }

                if (rampTimeInSamples <= 0) {
                    rampTimeInSamples = 1;
                }

                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->linetilde_21_activeRamps->push(destinationValue);
                this->linetilde_21_activeRamps->push(inc);
                this->linetilde_21_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
}

void numberobj_26_output_set(number v) {
    {
        list converted = {v};
        this->linetilde_21_segments_set(converted);
    }
}

void numberobj_26_value_set(number v) {
    this->numberobj_26_value_setter(v);
    v = this->numberobj_26_value;
    number localvalue = v;

    if (this->numberobj_26_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-155"), localvalue, this->_currentTime);
    this->numberobj_26_output_set(localvalue);
}

void codebox_01_out12_set(number v) {
    this->codebox_01_out12 = v;
    this->numberobj_26_value_set(this->codebox_01_out12);
}

void expr_12_in2_set(number v) {
    this->expr_12_in2 = v;
}

void trigger_02_out2_set(number v) {
    this->expr_12_in2_set(v);
}

void expr_12_in1_bang() {
    this->expr_12_out1_set(this->expr_12_in1 + this->expr_12_in2);//#map:+_obj-79:1
}

void trigger_02_out1_bang() {
    this->expr_12_in1_bang();
}

void trigger_02_input_number_set(number v) {
    this->trigger_02_out2_set(v);
    this->trigger_02_out1_bang();
}

void numberobj_27_output_set(number v) {
    this->trigger_02_input_number_set(v);
}

void numberobj_27_value_set(number v) {
    this->numberobj_27_value_setter(v);
    v = this->numberobj_27_value;
    number localvalue = v;

    if (this->numberobj_27_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-172"), localvalue, this->_currentTime);
    this->numberobj_27_output_set(localvalue);
}

void codebox_01_out13_set(number v) {
    this->codebox_01_out13 = v;
    this->numberobj_27_value_set(this->codebox_01_out13);
}

void linetilde_05_time_set(number v) {
    this->linetilde_05_time = v;
}

void linetilde_05_segments_set(const list& v) {
    this->linetilde_05_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        if (v->length == 1 && this->linetilde_05_time == 0) {
            this->linetilde_05_activeRamps->length = 0;
            this->linetilde_05_currentValue = v[0];
        } else {
            auto currentTime = this->currentsampletime();
            number lastRampValue = this->linetilde_05_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

            for (Index i = 0; i < this->linetilde_05_activeRamps->length; i += 3) {
                rampEnd = this->linetilde_05_activeRamps[(Index)(i + 2)];

                if (rampEnd > currentTime) {
                    this->linetilde_05_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->linetilde_05_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->linetilde_05_activeRamps[(Index)i] - valueDiff;
                    this->linetilde_05_activeRamps[(Index)i] = lastRampValue;
                    this->linetilde_05_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->linetilde_05_activeRamps[(Index)i];
                }
            }

            if (rampEnd < currentTime) {
                this->linetilde_05_activeRamps->push(lastRampValue);
                this->linetilde_05_activeRamps->push(0);
                this->linetilde_05_activeRamps->push(currentTime);
            }

            number lastRampEnd = currentTime;

            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;

                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);

                    if ((bool)(this->linetilde_05_keepramp)) {
                        this->linetilde_05_time_set(v[(Index)(i + 1)]);
                    }
                } else {
                    rampTimeInSamples = this->mstosamps(this->linetilde_05_time);
                }

                if (rampTimeInSamples <= 0) {
                    rampTimeInSamples = 1;
                }

                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->linetilde_05_activeRamps->push(destinationValue);
                this->linetilde_05_activeRamps->push(inc);
                this->linetilde_05_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
}

void numberobj_08_output_set(number v) {
    {
        list converted = {v};
        this->linetilde_05_segments_set(converted);
    }
}

void numberobj_08_value_set(number v) {
    this->numberobj_08_value_setter(v);
    v = this->numberobj_08_value;
    number localvalue = v;

    if (this->numberobj_08_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-86"), localvalue, this->_currentTime);
    this->numberobj_08_output_set(localvalue);
}

void expr_06_out1_set(number v) {
    this->expr_06_out1 = v;
    this->numberobj_08_value_set(this->expr_06_out1);
}

void expr_06_in1_set(number in1) {
    this->expr_06_in1 = in1;
    this->expr_06_out1_set(rnbo_pow(10, this->expr_06_in1 * 0.05));//#map:dbtoa_obj-84:1
}

void expr_05_out1_set(number v) {
    this->expr_05_out1 = v;
    this->expr_06_in1_set(this->expr_05_out1);
}

void expr_05_in1_set(number in1) {
    this->expr_05_in1 = in1;
    this->expr_05_out1_set(this->expr_05_in1 - this->expr_05_in2);//#map:-_obj-175:1
}

void numberobj_07_output_set(number v) {
    this->expr_05_in1_set(v);
}

void numberobj_07_value_set(number v) {
    this->numberobj_07_value_setter(v);
    v = this->numberobj_07_value;
    number localvalue = v;

    if (this->numberobj_07_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-73"), localvalue, this->_currentTime);
    this->numberobj_07_output_set(localvalue);
}

void numberobj_28_output_set(number v) {
    this->numberobj_07_value_set(v);
}

void numberobj_28_value_set(number v) {
    this->numberobj_28_value_setter(v);
    v = this->numberobj_28_value;
    number localvalue = v;

    if (this->numberobj_28_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-158"), localvalue, this->_currentTime);
    this->numberobj_28_output_set(localvalue);
}

void codebox_01_out14_set(number v) {
    this->codebox_01_out14 = v;
    this->numberobj_28_value_set(this->codebox_01_out14);
}

void expr_11_in2_set(number v) {
    this->expr_11_in2 = v;
}

void trigger_01_out2_set(number v) {
    this->expr_11_in2_set(v);
}

void expr_11_in1_bang() {
    this->expr_11_out1_set(this->expr_11_in1 + this->expr_11_in2);//#map:+_obj-177:1
}

void trigger_01_out1_bang() {
    this->expr_11_in1_bang();
}

void trigger_01_input_number_set(number v) {
    this->trigger_01_out2_set(v);
    this->trigger_01_out1_bang();
}

void numberobj_29_output_set(number v) {
    this->trigger_01_input_number_set(v);
}

void numberobj_29_value_set(number v) {
    this->numberobj_29_value_setter(v);
    v = this->numberobj_29_value;
    number localvalue = v;

    if (this->numberobj_29_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-179"), localvalue, this->_currentTime);
    this->numberobj_29_output_set(localvalue);
}

void codebox_01_out15_set(number v) {
    this->codebox_01_out15 = v;
    this->numberobj_29_value_set(this->codebox_01_out15);
}

void codebox_01_in1_set(number in1) {
    this->codebox_01_in1 = in1;
    number main_drive = (this->codebox_01_in1 > 1 ? 1 : (this->codebox_01_in1 < 0 ? 0 : this->codebox_01_in1));
    number pos = main_drive * 4;
    number seg = rnbo_floor(pos);
    seg = this->minimum(seg, 3);//#map:codebox_obj-168:25
    number t = pos - seg;
    number sa = (seg == 0 ? 1.34 : (seg == 1 ? 1.66 : (seg == 2 ? 1.54 : 4.88)));
    number sb = (seg == 0 ? 1.66 : (seg == 1 ? 1.54 : (seg == 2 ? 4.88 : 5.33)));
    number soft_pos = sa + (sb - sa) * t;
    sa = (seg == 0 ? 1.337 : (seg == 1 ? 1.503 : (seg == 2 ? 1.529 : 4.849)));//#map:codebox_obj-168:38
    sb = (seg == 0 ? 1.503 : (seg == 1 ? 1.529 : (seg == 2 ? 4.849 : 5.17)));//#map:codebox_obj-168:39
    number soft_neg = sa + (sb - sa) * t;
    sa = (seg == 0 ? 8.4 : (seg == 1 ? 8.1 : (seg == 2 ? 8 : 9.1)));//#map:codebox_obj-168:45
    sb = (seg == 0 ? 8.1 : (seg == 1 ? 8 : (seg == 2 ? 9.1 : 9.1)));//#map:codebox_obj-168:46
    number hard_pos = sa + (sb - sa) * t;
    sa = (seg == 0 ? 6.71 : (seg == 1 ? 6.045 : (seg == 2 ? 0.292 : 0.202)));//#map:codebox_obj-168:52
    sb = (seg == 0 ? 6.045 : (seg == 1 ? 0.292 : (seg == 2 ? 0.202 : 0.26)));//#map:codebox_obj-168:53
    number hard_neg = sa + (sb - sa) * t;
    sa = (seg == 0 ? 0.975 : (seg == 1 ? 0.66 : (seg == 2 ? 0.343 : 0.614)));//#map:codebox_obj-168:59
    sb = (seg == 0 ? 0.66 : (seg == 1 ? 0.343 : (seg == 2 ? 0.614 : 0.7)));//#map:codebox_obj-168:60
    number level_amt = sa + (sb - sa) * t;
    sa = (seg == 0 ? 0.55 : (seg == 1 ? 0.779 : (seg == 2 ? 0.86 : 0.85)));//#map:codebox_obj-168:66
    sb = (seg == 0 ? 0.779 : (seg == 1 ? 0.86 : (seg == 2 ? 0.85 : 0.85)));//#map:codebox_obj-168:67
    number headpump = sa + (sb - sa) * t;
    sa = (seg == 0 ? 0.67 : (seg == 1 ? 0.679 : (seg == 2 ? 0.679 : 0.701)));//#map:codebox_obj-168:73
    sb = (seg == 0 ? 0.679 : (seg == 1 ? 0.679 : (seg == 2 ? 0.701 : 0.69)));//#map:codebox_obj-168:74
    number pressure = sa + (sb - sa) * t;
    sa = (seg == 0 ? 0.042 : (seg == 1 ? 0.05 : (seg == 2 ? 0.009 : 0.216)));//#map:codebox_obj-168:80
    sb = (seg == 0 ? 0.05 : (seg == 1 ? 0.009 : (seg == 2 ? 0.216 : 0.24)));//#map:codebox_obj-168:81
    number rbass = sa + (sb - sa) * t;
    sa = (seg == 0 ? -20 : (seg == 1 ? -11.7 : (seg == 2 ? -3 : 0.47)));//#map:codebox_obj-168:87
    sb = (seg == 0 ? -11.7 : (seg == 1 ? -3 : (seg == 2 ? 0.47 : 6)));//#map:codebox_obj-168:88
    number trim = sa + (sb - sa) * t;
    sa = (seg == 0 ? 330 : (seg == 1 ? 324 : (seg == 2 ? 320 : 345)));//#map:codebox_obj-168:94
    sb = (seg == 0 ? 324 : (seg == 1 ? 320 : (seg == 2 ? 345 : 390)));//#map:codebox_obj-168:95
    number bass_fc = sa + (sb - sa) * t;
    sa = (seg == 0 ? 0.66 : (seg == 1 ? 0.76 : (seg == 2 ? 0.59 : 0.58)));//#map:codebox_obj-168:101
    sb = (seg == 0 ? 0.76 : (seg == 1 ? 0.59 : (seg == 2 ? 0.58 : 0.62)));//#map:codebox_obj-168:102
    number bass_relief = sa + (sb - sa) * t;
    sa = (seg == 0 ? 6 : (seg == 1 ? 3.8 : (seg == 2 ? 2.4 : 2.79)));//#map:codebox_obj-168:108
    sb = (seg == 0 ? 3.8 : (seg == 1 ? 2.4 : (seg == 2 ? 2.79 : 2.12)));//#map:codebox_obj-168:109
    number bass_return = sa + (sb - sa) * t;
    sa = (seg == 0 ? 0 : (seg == 1 ? 2 : (seg == 2 ? 3.2 : 1.8)));//#map:codebox_obj-168:115
    sb = (seg == 0 ? 2 : (seg == 1 ? 3.2 : (seg == 2 ? 1.8 : 2)));//#map:codebox_obj-168:116
    number bass_val = sa + (sb - sa) * t;
    sa = (seg == 0 ? -2.5 : (seg == 1 ? -10.5 : (seg == 2 ? -16 : -18.5)));//#map:codebox_obj-168:122
    sb = (seg == 0 ? -10.5 : (seg == 1 ? -16 : (seg == 2 ? -18.5 : -21)));//#map:codebox_obj-168:123
    number autogain = sa + (sb - sa) * t;
    sa = (seg == 0 ? 0 : (seg == 1 ? 0 : (seg == 2 ? -0.2 : -0.6)));//#map:codebox_obj-168:127
    sb = (seg == 0 ? 0 : (seg == 1 ? -0.2 : (seg == 2 ? -0.6 : -1)));//#map:codebox_obj-168:128
    number hf_trim = sa + (sb - sa) * t;
    this->codebox_01_out1_set(soft_pos);//#map:codebox_obj-168:135
    this->codebox_01_out2_set(soft_neg);//#map:codebox_obj-168:136
    this->codebox_01_out3_set(hard_pos);//#map:codebox_obj-168:137
    this->codebox_01_out4_set(hard_neg);//#map:codebox_obj-168:138
    this->codebox_01_out5_set(level_amt);//#map:codebox_obj-168:139
    this->codebox_01_out6_set(headpump);//#map:codebox_obj-168:140
    this->codebox_01_out7_set(pressure);//#map:codebox_obj-168:141
    this->codebox_01_out8_set(rbass);//#map:codebox_obj-168:142
    this->codebox_01_out9_set(trim);//#map:codebox_obj-168:143
    this->codebox_01_out10_set(bass_fc);//#map:codebox_obj-168:144
    this->codebox_01_out11_set(bass_relief);//#map:codebox_obj-168:145
    this->codebox_01_out12_set(bass_return);//#map:codebox_obj-168:146
    this->codebox_01_out13_set(bass_val);//#map:codebox_obj-168:147
    this->codebox_01_out14_set(autogain);//#map:codebox_obj-168:148
    this->codebox_01_out15_set(hf_trim);//#map:codebox_obj-168:149
}

void outport_01_input_number_set(number v) {
    this->getEngine()->sendNumMessage(TAG("mod"), TAG(""), v, this->_currentTime);
}

void numberobj_11_output_set(number ) {}

void numberobj_11_value_set(number v) {
    this->numberobj_11_value_setter(v);
    v = this->numberobj_11_value;
    number localvalue = v;

    if (this->numberobj_11_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-27"), localvalue, this->_currentTime);
    this->numberobj_11_output_set(localvalue);
}

void expr_10_out1_set(number v) {
    this->expr_10_out1 = v;
    this->outport_01_input_number_set(this->expr_10_out1);
    this->numberobj_11_value_set(this->expr_10_out1);
}

void expr_10_in1_set(number in1) {
    this->expr_10_in1 = in1;

    this->expr_10_out1_set(
        (this->expr_10_in1 > this->expr_10_in3 ? this->expr_10_in3 : (this->expr_10_in1 < this->expr_10_in2 ? this->expr_10_in2 : this->expr_10_in1))
    );//#map:clip_obj-25:1
}

void expr_10_in2_set(number v) {
    this->expr_10_in2 = v;
}

void expr_10_in3_set(number v) {
    this->expr_10_in3 = v;
}

void scale_01_out_set(const list& v) {
    this->scale_01_out = jsCreateListCopy(v);

    {
        if (v->length > 2)
            this->expr_10_in3_set(v[2]);

        if (v->length > 1)
            this->expr_10_in2_set(v[1]);

        number converted = (v->length > 0 ? v[0] : 0);
        this->expr_10_in1_set(converted);
    }
}

void scale_01_input_set(const list& v) {
    this->scale_01_input = jsCreateListCopy(v);
    list tmp = {};

    for (Index i = 0; i < v->length; i++) {
        tmp->push(this->scale(
            v[(Index)i],
            this->scale_01_inlow,
            this->scale_01_inhigh,
            this->scale_01_outlow,
            this->scale_01_outhigh,
            this->scale_01_power
        ));
    }

    this->scale_01_out_set(tmp);
}

void snapshot_01_input_bang_bang() {
    this->snapshot_01_out_set(this->snapshot_01_lastValue);
}

void button_01_output_bang() {
    this->snapshot_01_input_bang_bang();
}

void button_01_bangval_bang() {
    this->getEngine()->sendBangMessage(TAG("bangout"), TAG("button_obj-20"), this->_currentTime);;
    this->button_01_output_bang();
}

void inport_01_out_bang_bang() {
    this->button_01_bangval_bang();
}

void inport_01_out_number_set(number v) {
    RNBO_UNUSED(v);
    this->button_01_bangval_bang();
}

void inport_01_out_list_set(const list& v) {
    RNBO_UNUSED(v);
    this->button_01_bangval_bang();
}

void linetilde_05_perform(SampleValue * out, Index n) {
    auto __linetilde_05_time = this->linetilde_05_time;
    auto __linetilde_05_keepramp = this->linetilde_05_keepramp;
    auto __linetilde_05_currentValue = this->linetilde_05_currentValue;
    Index i = 0;

    if ((bool)(this->linetilde_05_activeRamps->length)) {
        while ((bool)(this->linetilde_05_activeRamps->length) && i < n) {
            number destinationValue = this->linetilde_05_activeRamps[0];
            number inc = this->linetilde_05_activeRamps[1];
            number rampTimeInSamples = this->linetilde_05_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __linetilde_05_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->linetilde_05_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->linetilde_05_activeRamps->length))) {
                    this->getEngine()->scheduleClockEventWithValue(
                        this,
                        -1580124982,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;

                    if ((bool)(!(bool)(__linetilde_05_keepramp))) {
                        __linetilde_05_time = 0;
                    }
                }
            }

            __linetilde_05_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __linetilde_05_currentValue;
        i++;
    }

    this->linetilde_05_currentValue = __linetilde_05_currentValue;
    this->linetilde_05_time = __linetilde_05_time;
}

void linetilde_06_perform(SampleValue * out, Index n) {
    auto __linetilde_06_time = this->linetilde_06_time;
    auto __linetilde_06_keepramp = this->linetilde_06_keepramp;
    auto __linetilde_06_currentValue = this->linetilde_06_currentValue;
    Index i = 0;

    if ((bool)(this->linetilde_06_activeRamps->length)) {
        while ((bool)(this->linetilde_06_activeRamps->length) && i < n) {
            number destinationValue = this->linetilde_06_activeRamps[0];
            number inc = this->linetilde_06_activeRamps[1];
            number rampTimeInSamples = this->linetilde_06_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __linetilde_06_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->linetilde_06_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->linetilde_06_activeRamps->length))) {
                    this->getEngine()->scheduleClockEventWithValue(
                        this,
                        1920515087,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;

                    if ((bool)(!(bool)(__linetilde_06_keepramp))) {
                        __linetilde_06_time = 0;
                    }
                }
            }

            __linetilde_06_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __linetilde_06_currentValue;
        i++;
    }

    this->linetilde_06_currentValue = __linetilde_06_currentValue;
    this->linetilde_06_time = __linetilde_06_time;
}

void dspexpr_03_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void dspexpr_07_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void dspexpr_13_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void dspexpr_12_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
    RNBO_UNUSED(in2);
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * 0.5;//#map:_###_obj_###_:1
    }
}

void dspexpr_15_perform(const Sample * in1, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = rnbo_abs(in1[(Index)i]);//#map:_###_obj_###_:1
    }
}

void slide_tilde_01_perform(const Sample * x, number up, number down, SampleValue * out1, Index n) {
    RNBO_UNUSED(down);
    RNBO_UNUSED(up);
    auto __slide_tilde_01_prev = this->slide_tilde_01_prev;
    auto iup = this->safediv(1., this->maximum(1., rnbo_abs(50)));
    auto idown = this->safediv(1., this->maximum(1., rnbo_abs(2000)));
    Index i;

    for (i = 0; i < n; i++) {
        number temp = x[(Index)i] - __slide_tilde_01_prev;
        __slide_tilde_01_prev = __slide_tilde_01_prev + ((x[(Index)i] > __slide_tilde_01_prev ? iup : idown)) * temp;
        out1[(Index)i] = __slide_tilde_01_prev;
    }

    this->slide_tilde_01_prev = __slide_tilde_01_prev;
}

void dspexpr_14_perform(const Sample * in1, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = (in1[(Index)i] <= 0 ? -999 : 20 * rnbo_log10((in1[(Index)i] <= 0.0000000001 ? 0.0000000001 : in1[(Index)i])));//#map:_###_obj_###_:1
    }
}

void snapshot_01_perform(const SampleValue * input_signal, Index n) {
    auto __snapshot_01_lastValue = this->snapshot_01_lastValue;
    auto __snapshot_01_calc = this->snapshot_01_calc;
    auto __snapshot_01_count = this->snapshot_01_count;
    auto __snapshot_01_nextTime = this->snapshot_01_nextTime;
    auto __snapshot_01_interval = this->snapshot_01_interval;
    number timeInSamples = this->msToSamps(__snapshot_01_interval, this->sr);

    if (__snapshot_01_interval > 0) {
        for (Index i = 0; i < n; i++) {
            if (__snapshot_01_nextTime <= __snapshot_01_count + (SampleIndex)(i)) {
                {
                    __snapshot_01_calc = input_signal[(Index)i];
                }

                this->getEngine()->scheduleClockEventWithValue(
                    this,
                    1646922831,
                    this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                    __snapshot_01_calc
                );;

                __snapshot_01_calc = 0;
                __snapshot_01_nextTime += timeInSamples;
            }
        }

        __snapshot_01_count += this->vs;
    }

    __snapshot_01_lastValue = input_signal[(Index)(n - 1)];
    this->snapshot_01_nextTime = __snapshot_01_nextTime;
    this->snapshot_01_count = __snapshot_01_count;
    this->snapshot_01_calc = __snapshot_01_calc;
    this->snapshot_01_lastValue = __snapshot_01_lastValue;
}

void linetilde_07_perform(SampleValue * out, Index n) {
    auto __linetilde_07_time = this->linetilde_07_time;
    auto __linetilde_07_keepramp = this->linetilde_07_keepramp;
    auto __linetilde_07_currentValue = this->linetilde_07_currentValue;
    Index i = 0;

    if ((bool)(this->linetilde_07_activeRamps->length)) {
        while ((bool)(this->linetilde_07_activeRamps->length) && i < n) {
            number destinationValue = this->linetilde_07_activeRamps[0];
            number inc = this->linetilde_07_activeRamps[1];
            number rampTimeInSamples = this->linetilde_07_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __linetilde_07_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->linetilde_07_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->linetilde_07_activeRamps->length))) {
                    this->getEngine()->scheduleClockEventWithValue(
                        this,
                        -21003509,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;

                    if ((bool)(!(bool)(__linetilde_07_keepramp))) {
                        __linetilde_07_time = 0;
                    }
                }
            }

            __linetilde_07_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __linetilde_07_currentValue;
        i++;
    }

    this->linetilde_07_currentValue = __linetilde_07_currentValue;
    this->linetilde_07_time = __linetilde_07_time;
}

void linetilde_08_perform(SampleValue * out, Index n) {
    auto __linetilde_08_time = this->linetilde_08_time;
    auto __linetilde_08_keepramp = this->linetilde_08_keepramp;
    auto __linetilde_08_currentValue = this->linetilde_08_currentValue;
    Index i = 0;

    if ((bool)(this->linetilde_08_activeRamps->length)) {
        while ((bool)(this->linetilde_08_activeRamps->length) && i < n) {
            number destinationValue = this->linetilde_08_activeRamps[0];
            number inc = this->linetilde_08_activeRamps[1];
            number rampTimeInSamples = this->linetilde_08_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __linetilde_08_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->linetilde_08_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->linetilde_08_activeRamps->length))) {
                    this->getEngine()->scheduleClockEventWithValue(
                        this,
                        1538117964,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;

                    if ((bool)(!(bool)(__linetilde_08_keepramp))) {
                        __linetilde_08_time = 0;
                    }
                }
            }

            __linetilde_08_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __linetilde_08_currentValue;
        i++;
    }

    this->linetilde_08_currentValue = __linetilde_08_currentValue;
    this->linetilde_08_time = __linetilde_08_time;
}

void numbertilde_03_perform(const SampleValue * input_signal, SampleValue * output, Index n) {
    auto __numbertilde_03_currentIntervalInSamples = this->numbertilde_03_currentIntervalInSamples;
    auto __numbertilde_03_lastValue = this->numbertilde_03_lastValue;
    auto __numbertilde_03_currentInterval = this->numbertilde_03_currentInterval;
    auto __numbertilde_03_rampInSamples = this->numbertilde_03_rampInSamples;
    auto __numbertilde_03_outValue = this->numbertilde_03_outValue;
    auto __numbertilde_03_currentMode = this->numbertilde_03_currentMode;
    number monitorvalue = input_signal[0];

    for (Index i = 0; i < n; i++) {
        if (__numbertilde_03_currentMode == 0) {
            output[(Index)i] = this->numbertilde_03_smooth_next(
                __numbertilde_03_outValue,
                __numbertilde_03_rampInSamples,
                __numbertilde_03_rampInSamples
            );
        } else {
            output[(Index)i] = input_signal[(Index)i];
        }
    }

    __numbertilde_03_currentInterval -= n;

    if (monitorvalue != __numbertilde_03_lastValue && __numbertilde_03_currentInterval <= 0) {
        __numbertilde_03_currentInterval = __numbertilde_03_currentIntervalInSamples;

        this->getEngine()->scheduleClockEventWithValue(
            this,
            -1494586265,
            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
            monitorvalue
        );;

        __numbertilde_03_lastValue = monitorvalue;
        this->getEngine()->sendListMessage(TAG("monitor"), TAG("number~_obj-95"), {monitorvalue}, this->_currentTime);;
    }

    this->numbertilde_03_currentInterval = __numbertilde_03_currentInterval;
    this->numbertilde_03_lastValue = __numbertilde_03_lastValue;
}

void linetilde_09_perform(SampleValue * out, Index n) {
    auto __linetilde_09_time = this->linetilde_09_time;
    auto __linetilde_09_keepramp = this->linetilde_09_keepramp;
    auto __linetilde_09_currentValue = this->linetilde_09_currentValue;
    Index i = 0;

    if ((bool)(this->linetilde_09_activeRamps->length)) {
        while ((bool)(this->linetilde_09_activeRamps->length) && i < n) {
            number destinationValue = this->linetilde_09_activeRamps[0];
            number inc = this->linetilde_09_activeRamps[1];
            number rampTimeInSamples = this->linetilde_09_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __linetilde_09_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->linetilde_09_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->linetilde_09_activeRamps->length))) {
                    this->getEngine()->scheduleClockEventWithValue(
                        this,
                        -1197727859,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;

                    if ((bool)(!(bool)(__linetilde_09_keepramp))) {
                        __linetilde_09_time = 0;
                    }
                }
            }

            __linetilde_09_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __linetilde_09_currentValue;
        i++;
    }

    this->linetilde_09_currentValue = __linetilde_09_currentValue;
    this->linetilde_09_time = __linetilde_09_time;
}

void ip_01_perform(SampleValue * out, Index n) {
    auto __ip_01_lastValue = this->ip_01_lastValue;
    auto __ip_01_lastIndex = this->ip_01_lastIndex;

    for (Index i = 0; i < n; i++) {
        out[(Index)i] = ((SampleIndex)(i) >= __ip_01_lastIndex ? __ip_01_lastValue : this->ip_01_sigbuf[(Index)i]);
    }

    __ip_01_lastIndex = 0;
    this->ip_01_lastIndex = __ip_01_lastIndex;
}

void linetilde_10_perform(SampleValue * out, Index n) {
    auto __linetilde_10_time = this->linetilde_10_time;
    auto __linetilde_10_keepramp = this->linetilde_10_keepramp;
    auto __linetilde_10_currentValue = this->linetilde_10_currentValue;
    Index i = 0;

    if ((bool)(this->linetilde_10_activeRamps->length)) {
        while ((bool)(this->linetilde_10_activeRamps->length) && i < n) {
            number destinationValue = this->linetilde_10_activeRamps[0];
            number inc = this->linetilde_10_activeRamps[1];
            number rampTimeInSamples = this->linetilde_10_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __linetilde_10_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->linetilde_10_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->linetilde_10_activeRamps->length))) {
                    this->getEngine()->scheduleClockEventWithValue(
                        this,
                        361393614,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;

                    if ((bool)(!(bool)(__linetilde_10_keepramp))) {
                        __linetilde_10_time = 0;
                    }
                }
            }

            __linetilde_10_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __linetilde_10_currentValue;
        i++;
    }

    this->linetilde_10_currentValue = __linetilde_10_currentValue;
    this->linetilde_10_time = __linetilde_10_time;
}

void dspexpr_11_perform(
    const Sample * in1,
    const Sample * in2,
    const Sample * in3,
    SampleValue * out1,
    Index n
) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] + in3[(Index)i] * (in2[(Index)i] - in1[(Index)i]);//#map:_###_obj_###_:1
    }
}

void linetilde_11_perform(SampleValue * out, Index n) {
    auto __linetilde_11_time = this->linetilde_11_time;
    auto __linetilde_11_keepramp = this->linetilde_11_keepramp;
    auto __linetilde_11_currentValue = this->linetilde_11_currentValue;
    Index i = 0;

    if ((bool)(this->linetilde_11_activeRamps->length)) {
        while ((bool)(this->linetilde_11_activeRamps->length) && i < n) {
            number destinationValue = this->linetilde_11_activeRamps[0];
            number inc = this->linetilde_11_activeRamps[1];
            number rampTimeInSamples = this->linetilde_11_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __linetilde_11_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->linetilde_11_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->linetilde_11_activeRamps->length))) {
                    this->getEngine()->scheduleClockEventWithValue(
                        this,
                        -815330736,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;

                    if ((bool)(!(bool)(__linetilde_11_keepramp))) {
                        __linetilde_11_time = 0;
                    }
                }
            }

            __linetilde_11_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __linetilde_11_currentValue;
        i++;
    }

    this->linetilde_11_currentValue = __linetilde_11_currentValue;
    this->linetilde_11_time = __linetilde_11_time;
}

void linetilde_12_perform(SampleValue * out, Index n) {
    auto __linetilde_12_time = this->linetilde_12_time;
    auto __linetilde_12_keepramp = this->linetilde_12_keepramp;
    auto __linetilde_12_currentValue = this->linetilde_12_currentValue;
    Index i = 0;

    if ((bool)(this->linetilde_12_activeRamps->length)) {
        while ((bool)(this->linetilde_12_activeRamps->length) && i < n) {
            number destinationValue = this->linetilde_12_activeRamps[0];
            number inc = this->linetilde_12_activeRamps[1];
            number rampTimeInSamples = this->linetilde_12_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __linetilde_12_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->linetilde_12_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->linetilde_12_activeRamps->length))) {
                    this->getEngine()->scheduleClockEventWithValue(
                        this,
                        743790737,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;

                    if ((bool)(!(bool)(__linetilde_12_keepramp))) {
                        __linetilde_12_time = 0;
                    }
                }
            }

            __linetilde_12_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __linetilde_12_currentValue;
        i++;
    }

    this->linetilde_12_currentValue = __linetilde_12_currentValue;
    this->linetilde_12_time = __linetilde_12_time;
}

void linetilde_13_perform(SampleValue * out, Index n) {
    auto __linetilde_13_time = this->linetilde_13_time;
    auto __linetilde_13_keepramp = this->linetilde_13_keepramp;
    auto __linetilde_13_currentValue = this->linetilde_13_currentValue;
    Index i = 0;

    if ((bool)(this->linetilde_13_activeRamps->length)) {
        while ((bool)(this->linetilde_13_activeRamps->length) && i < n) {
            number destinationValue = this->linetilde_13_activeRamps[0];
            number inc = this->linetilde_13_activeRamps[1];
            number rampTimeInSamples = this->linetilde_13_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __linetilde_13_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->linetilde_13_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->linetilde_13_activeRamps->length))) {
                    this->getEngine()->scheduleClockEventWithValue(
                        this,
                        349887047,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;

                    if ((bool)(!(bool)(__linetilde_13_keepramp))) {
                        __linetilde_13_time = 0;
                    }
                }
            }

            __linetilde_13_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __linetilde_13_currentValue;
        i++;
    }

    this->linetilde_13_currentValue = __linetilde_13_currentValue;
    this->linetilde_13_time = __linetilde_13_time;
}

void linetilde_14_perform(SampleValue * out, Index n) {
    auto __linetilde_14_time = this->linetilde_14_time;
    auto __linetilde_14_keepramp = this->linetilde_14_keepramp;
    auto __linetilde_14_currentValue = this->linetilde_14_currentValue;
    Index i = 0;

    if ((bool)(this->linetilde_14_activeRamps->length)) {
        while ((bool)(this->linetilde_14_activeRamps->length) && i < n) {
            number destinationValue = this->linetilde_14_activeRamps[0];
            number inc = this->linetilde_14_activeRamps[1];
            number rampTimeInSamples = this->linetilde_14_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __linetilde_14_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->linetilde_14_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->linetilde_14_activeRamps->length))) {
                    this->getEngine()->scheduleClockEventWithValue(
                        this,
                        1909008520,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;

                    if ((bool)(!(bool)(__linetilde_14_keepramp))) {
                        __linetilde_14_time = 0;
                    }
                }
            }

            __linetilde_14_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __linetilde_14_currentValue;
        i++;
    }

    this->linetilde_14_currentValue = __linetilde_14_currentValue;
    this->linetilde_14_time = __linetilde_14_time;
}

void linetilde_15_perform(SampleValue * out, Index n) {
    auto __linetilde_15_time = this->linetilde_15_time;
    auto __linetilde_15_keepramp = this->linetilde_15_keepramp;
    auto __linetilde_15_currentValue = this->linetilde_15_currentValue;
    Index i = 0;

    if ((bool)(this->linetilde_15_activeRamps->length)) {
        while ((bool)(this->linetilde_15_activeRamps->length) && i < n) {
            number destinationValue = this->linetilde_15_activeRamps[0];
            number inc = this->linetilde_15_activeRamps[1];
            number rampTimeInSamples = this->linetilde_15_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __linetilde_15_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->linetilde_15_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->linetilde_15_activeRamps->length))) {
                    this->getEngine()->scheduleClockEventWithValue(
                        this,
                        -826837303,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;

                    if ((bool)(!(bool)(__linetilde_15_keepramp))) {
                        __linetilde_15_time = 0;
                    }
                }
            }

            __linetilde_15_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __linetilde_15_currentValue;
        i++;
    }

    this->linetilde_15_currentValue = __linetilde_15_currentValue;
    this->linetilde_15_time = __linetilde_15_time;
}

void linetilde_16_perform(SampleValue * out, Index n) {
    auto __linetilde_16_time = this->linetilde_16_time;
    auto __linetilde_16_keepramp = this->linetilde_16_keepramp;
    auto __linetilde_16_currentValue = this->linetilde_16_currentValue;
    Index i = 0;

    if ((bool)(this->linetilde_16_activeRamps->length)) {
        while ((bool)(this->linetilde_16_activeRamps->length) && i < n) {
            number destinationValue = this->linetilde_16_activeRamps[0];
            number inc = this->linetilde_16_activeRamps[1];
            number rampTimeInSamples = this->linetilde_16_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __linetilde_16_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->linetilde_16_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->linetilde_16_activeRamps->length))) {
                    this->getEngine()->scheduleClockEventWithValue(
                        this,
                        732284170,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;

                    if ((bool)(!(bool)(__linetilde_16_keepramp))) {
                        __linetilde_16_time = 0;
                    }
                }
            }

            __linetilde_16_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __linetilde_16_currentValue;
        i++;
    }

    this->linetilde_16_currentValue = __linetilde_16_currentValue;
    this->linetilde_16_time = __linetilde_16_time;
}

void linetilde_17_perform(SampleValue * out, Index n) {
    auto __linetilde_17_time = this->linetilde_17_time;
    auto __linetilde_17_keepramp = this->linetilde_17_keepramp;
    auto __linetilde_17_currentValue = this->linetilde_17_currentValue;
    Index i = 0;

    if ((bool)(this->linetilde_17_activeRamps->length)) {
        while ((bool)(this->linetilde_17_activeRamps->length) && i < n) {
            number destinationValue = this->linetilde_17_activeRamps[0];
            number inc = this->linetilde_17_activeRamps[1];
            number rampTimeInSamples = this->linetilde_17_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __linetilde_17_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->linetilde_17_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->linetilde_17_activeRamps->length))) {
                    this->getEngine()->scheduleClockEventWithValue(
                        this,
                        -2003561653,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;

                    if ((bool)(!(bool)(__linetilde_17_keepramp))) {
                        __linetilde_17_time = 0;
                    }
                }
            }

            __linetilde_17_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __linetilde_17_currentValue;
        i++;
    }

    this->linetilde_17_currentValue = __linetilde_17_currentValue;
    this->linetilde_17_time = __linetilde_17_time;
}

void gen_01_perform(
    const Sample * in1,
    const Sample * in2,
    const Sample * in3,
    const Sample * in4,
    SampleValue * out1,
    SampleValue * out2,
    Index n
) {
    auto __gen_01_press_high_r_value = this->gen_01_press_high_r_value;
    auto __gen_01_press_low_r_value = this->gen_01_press_low_r_value;
    auto __gen_01_press_high_l_value = this->gen_01_press_high_l_value;
    auto __gen_01_press_low_l_value = this->gen_01_press_low_l_value;
    auto __gen_01_hb_lp2_r_value = this->gen_01_hb_lp2_r_value;
    auto __gen_01_hb_lp_r_value = this->gen_01_hb_lp_r_value;
    auto __gen_01_hb_lp2_l_value = this->gen_01_hb_lp2_l_value;
    auto __gen_01_hb_lp_l_value = this->gen_01_hb_lp_l_value;
    number alpha_deep_4 = 1 - rnbo_exp((this->samplerate() == 0. ? 0. : -188.49555921539 / this->samplerate()));
    number alpha_press_low_11 = 1 - rnbo_exp((this->samplerate() == 0. ? 0. : -1476.5485471872 / this->samplerate()));
    number alpha_press_high_12 = 1 - rnbo_exp((this->samplerate() == 0. ? 0. : -25761.059759436 / this->samplerate()));
    Index i;

    for (i = 0; i < n; i++) {
        number xin_l_0 = in1[(Index)i];
        number xin_r_1 = in2[(Index)i];
        number eff_deep_2 = in3[(Index)i] * in3[(Index)i];
        number eff_pressure_3 = in4[(Index)i] * in4[(Index)i] * in4[(Index)i];
        __gen_01_hb_lp_l_value = __gen_01_hb_lp_l_value + alpha_deep_4 * (xin_l_0 - __gen_01_hb_lp_l_value);
        __gen_01_hb_lp2_l_value = __gen_01_hb_lp2_l_value + alpha_deep_4 * (__gen_01_hb_lp_l_value - __gen_01_hb_lp2_l_value);
        number hb_deep_l_5 = __gen_01_hb_lp_l_value - __gen_01_hb_lp2_l_value * 0.7;
        number hb_drive_l_6 = 1.15 + rnbo_abs(hb_deep_l_5) * 0.86;
        number hb_out_deep_l_7 = rnbo_tanh(hb_deep_l_5 * hb_drive_l_6) * 1.465;
        __gen_01_hb_lp_r_value = __gen_01_hb_lp_r_value + alpha_deep_4 * (xin_r_1 - __gen_01_hb_lp_r_value);
        __gen_01_hb_lp2_r_value = __gen_01_hb_lp2_r_value + alpha_deep_4 * (__gen_01_hb_lp_r_value - __gen_01_hb_lp2_r_value);
        number hb_deep_r_8 = __gen_01_hb_lp_r_value - __gen_01_hb_lp2_r_value * 0.7;
        number hb_drive_r_9 = 1.15 + rnbo_abs(hb_deep_r_8) * 0.86;
        number hb_out_deep_r_10 = rnbo_tanh(hb_deep_r_8 * hb_drive_r_9) * 1.465;
        __gen_01_press_low_l_value = __gen_01_press_low_l_value + alpha_press_low_11 * (xin_l_0 - __gen_01_press_low_l_value);
        __gen_01_press_high_l_value = __gen_01_press_high_l_value + alpha_press_high_12 * (xin_l_0 - __gen_01_press_high_l_value);
        number press_band_l_13 = __gen_01_press_high_l_value - __gen_01_press_low_l_value;
        number hp_out_l_14 = rnbo_tanh(press_band_l_13 * 1) * 1.3;
        __gen_01_press_low_r_value = __gen_01_press_low_r_value + alpha_press_low_11 * (xin_r_1 - __gen_01_press_low_r_value);
        __gen_01_press_high_r_value = __gen_01_press_high_r_value + alpha_press_high_12 * (xin_r_1 - __gen_01_press_high_r_value);
        number press_band_r_15 = __gen_01_press_high_r_value - __gen_01_press_low_r_value;
        number hp_out_r_16 = rnbo_tanh(press_band_r_15 * 1) * 1.3;
        number y_l_17 = xin_l_0 + hb_out_deep_l_7 * eff_deep_2 - hp_out_l_14 * eff_pressure_3;
        number y_r_18 = xin_r_1 + hb_out_deep_r_10 * eff_deep_2 - hp_out_r_16 * eff_pressure_3;
        number expr_1_19 = y_l_17;
        number expr_2_20 = y_r_18;
        out1[(Index)i] = expr_1_19;
        out2[(Index)i] = expr_2_20;
    }

    this->gen_01_hb_lp_l_value = __gen_01_hb_lp_l_value;
    this->gen_01_hb_lp2_l_value = __gen_01_hb_lp2_l_value;
    this->gen_01_hb_lp_r_value = __gen_01_hb_lp_r_value;
    this->gen_01_hb_lp2_r_value = __gen_01_hb_lp2_r_value;
    this->gen_01_press_low_l_value = __gen_01_press_low_l_value;
    this->gen_01_press_high_l_value = __gen_01_press_high_l_value;
    this->gen_01_press_low_r_value = __gen_01_press_low_r_value;
    this->gen_01_press_high_r_value = __gen_01_press_high_r_value;
}

void dspexpr_04_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void dspexpr_10_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void linetilde_18_perform(SampleValue * out, Index n) {
    auto __linetilde_18_time = this->linetilde_18_time;
    auto __linetilde_18_keepramp = this->linetilde_18_keepramp;
    auto __linetilde_18_currentValue = this->linetilde_18_currentValue;
    Index i = 0;

    if ((bool)(this->linetilde_18_activeRamps->length)) {
        while ((bool)(this->linetilde_18_activeRamps->length) && i < n) {
            number destinationValue = this->linetilde_18_activeRamps[0];
            number inc = this->linetilde_18_activeRamps[1];
            number rampTimeInSamples = this->linetilde_18_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __linetilde_18_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->linetilde_18_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->linetilde_18_activeRamps->length))) {
                    this->getEngine()->scheduleClockEventWithValue(
                        this,
                        -444440180,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;

                    if ((bool)(!(bool)(__linetilde_18_keepramp))) {
                        __linetilde_18_time = 0;
                    }
                }
            }

            __linetilde_18_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __linetilde_18_currentValue;
        i++;
    }

    this->linetilde_18_currentValue = __linetilde_18_currentValue;
    this->linetilde_18_time = __linetilde_18_time;
}

void linetilde_19_perform(SampleValue * out, Index n) {
    auto __linetilde_19_time = this->linetilde_19_time;
    auto __linetilde_19_keepramp = this->linetilde_19_keepramp;
    auto __linetilde_19_currentValue = this->linetilde_19_currentValue;
    Index i = 0;

    if ((bool)(this->linetilde_19_activeRamps->length)) {
        while ((bool)(this->linetilde_19_activeRamps->length) && i < n) {
            number destinationValue = this->linetilde_19_activeRamps[0];
            number inc = this->linetilde_19_activeRamps[1];
            number rampTimeInSamples = this->linetilde_19_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __linetilde_19_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->linetilde_19_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->linetilde_19_activeRamps->length))) {
                    this->getEngine()->scheduleClockEventWithValue(
                        this,
                        1114681293,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;

                    if ((bool)(!(bool)(__linetilde_19_keepramp))) {
                        __linetilde_19_time = 0;
                    }
                }
            }

            __linetilde_19_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __linetilde_19_currentValue;
        i++;
    }

    this->linetilde_19_currentValue = __linetilde_19_currentValue;
    this->linetilde_19_time = __linetilde_19_time;
}

void linetilde_20_perform(SampleValue * out, Index n) {
    auto __linetilde_20_time = this->linetilde_20_time;
    auto __linetilde_20_keepramp = this->linetilde_20_keepramp;
    auto __linetilde_20_currentValue = this->linetilde_20_currentValue;
    Index i = 0;

    if ((bool)(this->linetilde_20_activeRamps->length)) {
        while ((bool)(this->linetilde_20_activeRamps->length) && i < n) {
            number destinationValue = this->linetilde_20_activeRamps[0];
            number inc = this->linetilde_20_activeRamps[1];
            number rampTimeInSamples = this->linetilde_20_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __linetilde_20_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->linetilde_20_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->linetilde_20_activeRamps->length))) {
                    this->getEngine()->scheduleClockEventWithValue(
                        this,
                        -1621164530,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;

                    if ((bool)(!(bool)(__linetilde_20_keepramp))) {
                        __linetilde_20_time = 0;
                    }
                }
            }

            __linetilde_20_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __linetilde_20_currentValue;
        i++;
    }

    this->linetilde_20_currentValue = __linetilde_20_currentValue;
    this->linetilde_20_time = __linetilde_20_time;
}

void linetilde_21_perform(SampleValue * out, Index n) {
    auto __linetilde_21_time = this->linetilde_21_time;
    auto __linetilde_21_keepramp = this->linetilde_21_keepramp;
    auto __linetilde_21_currentValue = this->linetilde_21_currentValue;
    Index i = 0;

    if ((bool)(this->linetilde_21_activeRamps->length)) {
        while ((bool)(this->linetilde_21_activeRamps->length) && i < n) {
            number destinationValue = this->linetilde_21_activeRamps[0];
            number inc = this->linetilde_21_activeRamps[1];
            number rampTimeInSamples = this->linetilde_21_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __linetilde_21_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->linetilde_21_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->linetilde_21_activeRamps->length))) {
                    this->getEngine()->scheduleClockEventWithValue(
                        this,
                        -62043057,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;

                    if ((bool)(!(bool)(__linetilde_21_keepramp))) {
                        __linetilde_21_time = 0;
                    }
                }
            }

            __linetilde_21_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __linetilde_21_currentValue;
        i++;
    }

    this->linetilde_21_currentValue = __linetilde_21_currentValue;
    this->linetilde_21_time = __linetilde_21_time;
}

void gen_03_perform(
    const Sample * in1,
    const Sample * in2,
    const Sample * in3,
    const Sample * in4,
    const Sample * in5,
    const Sample * in6,
    const Sample * in7,
    const Sample * in8,
    const Sample * in9,
    const Sample * in10,
    SampleValue * out1,
    SampleValue * out2,
    Index n
) {
    auto __gen_03_y1_r_value = this->gen_03_y1_r_value;
    auto __gen_03_x1_r_value = this->gen_03_x1_r_value;
    auto __gen_03_y1_l_value = this->gen_03_y1_l_value;
    auto __gen_03_x1_l_value = this->gen_03_x1_l_value;
    auto __gen_03_lp_r_value = this->gen_03_lp_r_value;
    auto __gen_03_lp_l_value = this->gen_03_lp_l_value;
    Index i;

    for (i = 0; i < n; i++) {
        number xin_l_0 = in1[(Index)i];
        number xin_r_1 = in2[(Index)i];
        number soft_pos_2 = in3[(Index)i];
        number soft_neg_3 = in4[(Index)i];
        number hard_pos_4 = in5[(Index)i];
        number hard_neg_5 = in6[(Index)i];
        number level_amt_6 = in7[(Index)i];
        number bass_fc_7 = in8[(Index)i];
        number bass_relief_8 = in9[(Index)i];
        number bass_return_9 = in10[(Index)i];

        number alpha_lp_10 = 1 - rnbo_exp(
            (this->samplerate() == 0. ? 0. : -6.2831853071796 * bass_fc_7 / this->samplerate())
        );

        __gen_03_lp_l_value = __gen_03_lp_l_value + alpha_lp_10 * (xin_l_0 - __gen_03_lp_l_value);
        __gen_03_lp_r_value = __gen_03_lp_r_value + alpha_lp_10 * (xin_r_1 - __gen_03_lp_r_value);
        number pre_l_11 = xin_l_0 - __gen_03_lp_l_value * bass_relief_8;
        number pre_r_12 = xin_r_1 - __gen_03_lp_r_value * bass_relief_8;
        number x_l_13 = (pre_l_11 + 0.0056) * 18;
        number x_r_14 = (pre_r_12 + 0.0056) * 18;
        auto pos_l_15 = this->maximum(x_l_13, 0);
        auto neg_l_16 = this->minimum(x_l_13, 0);
        auto pos_r_17 = this->maximum(x_r_14, 0);
        auto neg_r_18 = this->minimum(x_r_14, 0);
        number level_l_19 = this->__wrapped_op_clamp(rnbo_abs(x_l_13) * level_amt_6, 0, 1);
        number level_r_20 = this->__wrapped_op_clamp(rnbo_abs(x_r_14) * level_amt_6, 0, 1);
        number level_l_1_21 = level_l_19 * level_l_19;
        number level_r_2_22 = level_r_20 * level_r_20;
        number soft_l_23 = rnbo_tanh(pos_l_15 * soft_pos_2) + rnbo_tanh(neg_l_16 * soft_neg_3);
        number soft_r_24 = rnbo_tanh(pos_r_17 * soft_pos_2) + rnbo_tanh(neg_r_18 * soft_neg_3);
        number hard_l_25 = rnbo_tanh(pos_l_15 * hard_pos_4 * 1.8) + rnbo_tanh(neg_l_16 * hard_neg_5 * 1.8);
        number hard_r_26 = rnbo_tanh(pos_r_17 * hard_pos_4 * 1.8) + rnbo_tanh(neg_r_18 * hard_neg_5 * 1.8);
        number sat_l_27 = soft_l_23 * (1 - level_l_1_21) + hard_l_25 * level_l_1_21;
        number sat_r_28 = soft_r_24 * (1 - level_r_2_22) + hard_r_26 * level_r_2_22;
        number bass_path_l_29 = __gen_03_lp_l_value * bass_return_9;
        number bass_path_r_30 = __gen_03_lp_r_value * bass_return_9;
        number bass_path_l_3_31 = rnbo_tanh(bass_path_l_29 * 0.1) / (number)0.1;
        number bass_path_r_4_32 = rnbo_tanh(bass_path_r_30 * 0.1) / (number)0.1;
        number y_l_33 = sat_l_27 + bass_path_l_3_31;
        number y_r_34 = sat_r_28 + bass_path_r_4_32;
        number y_l_5_35 = y_l_33 - 0.0056;
        number y_r_6_36 = y_r_34 - 0.0056;
        number out_l_37 = y_l_5_35 - __gen_03_x1_l_value + 0.995 * __gen_03_y1_l_value;
        __gen_03_x1_l_value = y_l_5_35;
        __gen_03_y1_l_value = out_l_37;
        number out_r_38 = y_r_6_36 - __gen_03_x1_r_value + 0.995 * __gen_03_y1_r_value;
        __gen_03_x1_r_value = y_r_6_36;
        __gen_03_y1_r_value = out_r_38;
        number expr_7_39 = out_l_37;
        number expr_8_40 = out_r_38;
        out2[(Index)i] = expr_8_40;
        out1[(Index)i] = expr_7_39;
    }

    this->gen_03_lp_l_value = __gen_03_lp_l_value;
    this->gen_03_lp_r_value = __gen_03_lp_r_value;
    this->gen_03_x1_l_value = __gen_03_x1_l_value;
    this->gen_03_y1_l_value = __gen_03_y1_l_value;
    this->gen_03_x1_r_value = __gen_03_x1_r_value;
    this->gen_03_y1_r_value = __gen_03_y1_r_value;
}

void gen_02_perform(
    const Sample * in1,
    const Sample * in2,
    const Sample * in3,
    SampleValue * out1,
    SampleValue * out2,
    Index n
) {
    auto __gen_02_ub_lp2_r_value = this->gen_02_ub_lp2_r_value;
    auto __gen_02_ub_lp2_l_value = this->gen_02_ub_lp2_l_value;
    auto __gen_02_ub_lp1_r_value = this->gen_02_ub_lp1_r_value;
    auto __gen_02_ub_lp1_l_value = this->gen_02_ub_lp1_l_value;
    number alpha1_2 = 1 - rnbo_exp((this->samplerate() == 0. ? 0. : -439.82297150257 / this->samplerate()));
    number alpha2_3 = 1 - rnbo_exp((this->samplerate() == 0. ? 0. : -1005.3096491487 / this->samplerate()));
    Index i;

    for (i = 0; i < n; i++) {
        number xin_l_0 = in1[(Index)i];
        number xin_r_1 = in2[(Index)i];
        __gen_02_ub_lp1_l_value = __gen_02_ub_lp1_l_value + alpha1_2 * (xin_l_0 - __gen_02_ub_lp1_l_value);
        __gen_02_ub_lp1_r_value = __gen_02_ub_lp1_r_value + alpha1_2 * (xin_r_1 - __gen_02_ub_lp1_r_value);
        __gen_02_ub_lp2_l_value = __gen_02_ub_lp2_l_value + alpha2_3 * (xin_l_0 - __gen_02_ub_lp2_l_value);
        __gen_02_ub_lp2_r_value = __gen_02_ub_lp2_r_value + alpha2_3 * (xin_r_1 - __gen_02_ub_lp2_r_value);
        number ub_band_l_4 = __gen_02_ub_lp2_l_value - __gen_02_ub_lp1_l_value;
        number ub_band_r_5 = __gen_02_ub_lp2_r_value - __gen_02_ub_lp1_r_value;
        number ub_comp_l_6 = rnbo_tanh(ub_band_l_4 * 30.8) / (number)30.8;
        number ub_comp_r_7 = rnbo_tanh(ub_band_r_5 * 30.8) / (number)30.8;
        number y_l_8 = xin_l_0 - ub_band_l_4 * in3[(Index)i] + ub_comp_l_6 * in3[(Index)i];
        number y_r_9 = xin_r_1 - ub_band_r_5 * in3[(Index)i] + ub_comp_r_7 * in3[(Index)i];
        number expr_1_10 = y_l_8;
        number expr_2_11 = y_r_9;
        out1[(Index)i] = expr_1_10;
        out2[(Index)i] = expr_2_11;
    }

    this->gen_02_ub_lp1_l_value = __gen_02_ub_lp1_l_value;
    this->gen_02_ub_lp1_r_value = __gen_02_ub_lp1_r_value;
    this->gen_02_ub_lp2_l_value = __gen_02_ub_lp2_l_value;
    this->gen_02_ub_lp2_r_value = __gen_02_ub_lp2_r_value;
}

void p_01_perform(
    const SampleValue * in1,
    const SampleValue * in2,
    SampleValue * out1,
    SampleValue * out2,
    Index n
) {
    // subpatcher: Bassshelf
    ConstSampleArray<2> ins = {in1, in2};

    SampleArray<2> outs = {out1, out2};
    this->p_01->process(ins, 2, outs, 2, n);
}

void p_02_perform(
    const SampleValue * in1,
    const SampleValue * in2,
    SampleValue * out1,
    SampleValue * out2,
    Index n
) {
    // subpatcher: Highshelf
    ConstSampleArray<2> ins = {in1, in2};

    SampleArray<2> outs = {out1, out2};
    this->p_02->process(ins, 2, outs, 2, n);
}

void dspexpr_05_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void dspexpr_06_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void onepole_tilde_01_perform(const Sample * x, number freqInHz, SampleValue * out1, Index n) {
    RNBO_UNUSED(freqInHz);
    auto __onepole_tilde_01_lastY = this->onepole_tilde_01_lastY;
    auto __onepole_tilde_01_b1 = this->onepole_tilde_01_b1;
    auto __onepole_tilde_01_a0 = this->onepole_tilde_01_a0;
    auto __onepole_tilde_01_needsUpdate = this->onepole_tilde_01_needsUpdate;
    auto __onepole_tilde_01_freq = this->onepole_tilde_01_freq;
    Index i;

    for (i = 0; i < n; i++) {
        if (__onepole_tilde_01_freq != 13000 || (bool)(__onepole_tilde_01_needsUpdate)) {
            __onepole_tilde_01_freq = 13000;
            __onepole_tilde_01_a0 = 1 - rnbo_exp(-81681.40899333463 / this->sr);
            __onepole_tilde_01_a0 = (__onepole_tilde_01_a0 > 0.99999 ? 0.99999 : (__onepole_tilde_01_a0 < 0.00001 ? 0.00001 : __onepole_tilde_01_a0));
            __onepole_tilde_01_b1 = 1 - __onepole_tilde_01_a0;
            __onepole_tilde_01_needsUpdate = false;
        }

        __onepole_tilde_01_lastY = __onepole_tilde_01_a0 * x[(Index)i] + __onepole_tilde_01_b1 * __onepole_tilde_01_lastY;
        out1[(Index)i] = __onepole_tilde_01_lastY;
    }

    this->onepole_tilde_01_freq = __onepole_tilde_01_freq;
    this->onepole_tilde_01_needsUpdate = __onepole_tilde_01_needsUpdate;
    this->onepole_tilde_01_a0 = __onepole_tilde_01_a0;
    this->onepole_tilde_01_b1 = __onepole_tilde_01_b1;
    this->onepole_tilde_01_lastY = __onepole_tilde_01_lastY;
}

void dspexpr_01_perform(
    const Sample * in1,
    const Sample * in2,
    const Sample * in3,
    SampleValue * out1,
    Index n
) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] + in3[(Index)i] * (in2[(Index)i] - in1[(Index)i]);//#map:_###_obj_###_:1
    }
}

void dspexpr_08_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void dspexpr_09_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void onepole_tilde_02_perform(const Sample * x, number freqInHz, SampleValue * out1, Index n) {
    RNBO_UNUSED(freqInHz);
    auto __onepole_tilde_02_lastY = this->onepole_tilde_02_lastY;
    auto __onepole_tilde_02_b1 = this->onepole_tilde_02_b1;
    auto __onepole_tilde_02_a0 = this->onepole_tilde_02_a0;
    auto __onepole_tilde_02_needsUpdate = this->onepole_tilde_02_needsUpdate;
    auto __onepole_tilde_02_freq = this->onepole_tilde_02_freq;
    Index i;

    for (i = 0; i < n; i++) {
        if (__onepole_tilde_02_freq != 13000 || (bool)(__onepole_tilde_02_needsUpdate)) {
            __onepole_tilde_02_freq = 13000;
            __onepole_tilde_02_a0 = 1 - rnbo_exp(-81681.40899333463 / this->sr);
            __onepole_tilde_02_a0 = (__onepole_tilde_02_a0 > 0.99999 ? 0.99999 : (__onepole_tilde_02_a0 < 0.00001 ? 0.00001 : __onepole_tilde_02_a0));
            __onepole_tilde_02_b1 = 1 - __onepole_tilde_02_a0;
            __onepole_tilde_02_needsUpdate = false;
        }

        __onepole_tilde_02_lastY = __onepole_tilde_02_a0 * x[(Index)i] + __onepole_tilde_02_b1 * __onepole_tilde_02_lastY;
        out1[(Index)i] = __onepole_tilde_02_lastY;
    }

    this->onepole_tilde_02_freq = __onepole_tilde_02_freq;
    this->onepole_tilde_02_needsUpdate = __onepole_tilde_02_needsUpdate;
    this->onepole_tilde_02_a0 = __onepole_tilde_02_a0;
    this->onepole_tilde_02_b1 = __onepole_tilde_02_b1;
    this->onepole_tilde_02_lastY = __onepole_tilde_02_lastY;
}

void dspexpr_02_perform(
    const Sample * in1,
    const Sample * in2,
    const Sample * in3,
    SampleValue * out1,
    Index n
) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] + in3[(Index)i] * (in2[(Index)i] - in1[(Index)i]);//#map:_###_obj_###_:1
    }
}

void stackprotect_perform(Index n) {
    RNBO_UNUSED(n);
    auto __stackprotect_count = this->stackprotect_count;
    __stackprotect_count = 0;
    this->stackprotect_count = __stackprotect_count;
}

void numberobj_07_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_07_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_07_value = localvalue;
}

void numberobj_08_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_08_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_08_value = localvalue;
}

void numberobj_09_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_09_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_09_value = localvalue;
}

void numberobj_10_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_10_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_10_value = localvalue;
}

void numberobj_11_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_11_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_11_value = localvalue;
}

void numberobj_12_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_12_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_12_value = localvalue;
}

void numberobj_13_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_13_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_13_value = localvalue;
}

void numberobj_14_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_14_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_14_value = localvalue;
}

void numberobj_15_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_15_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_15_value = localvalue;
}

void numberobj_16_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_16_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_16_value = localvalue;
}

void numberobj_17_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_17_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_17_value = localvalue;
}

void numberobj_18_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_18_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_18_value = localvalue;
}

void numberobj_19_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_19_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_19_value = localvalue;
}

void numberobj_20_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_20_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_20_value = localvalue;
}

void numberobj_21_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_21_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_21_value = localvalue;
}

void numberobj_22_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_22_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_22_value = localvalue;
}

void numberobj_23_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_23_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_23_value = localvalue;
}

void numberobj_24_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_24_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_24_value = localvalue;
}

void numberobj_25_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_25_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_25_value = localvalue;
}

void numberobj_26_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_26_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_26_value = localvalue;
}

void numberobj_27_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_27_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_27_value = localvalue;
}

void numberobj_28_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_28_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_28_value = localvalue;
}

void numberobj_29_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_29_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_29_value = localvalue;
}

number gen_01_hb_lp_l_getvalue() {
    return this->gen_01_hb_lp_l_value;
}

void gen_01_hb_lp_l_setvalue(number val) {
    this->gen_01_hb_lp_l_value = val;
}

void gen_01_hb_lp_l_reset() {
    this->gen_01_hb_lp_l_value = 0;
}

void gen_01_hb_lp_l_init() {
    this->gen_01_hb_lp_l_value = 0;
}

number gen_01_hb_lp2_l_getvalue() {
    return this->gen_01_hb_lp2_l_value;
}

void gen_01_hb_lp2_l_setvalue(number val) {
    this->gen_01_hb_lp2_l_value = val;
}

void gen_01_hb_lp2_l_reset() {
    this->gen_01_hb_lp2_l_value = 0;
}

void gen_01_hb_lp2_l_init() {
    this->gen_01_hb_lp2_l_value = 0;
}

number gen_01_hb_lp_r_getvalue() {
    return this->gen_01_hb_lp_r_value;
}

void gen_01_hb_lp_r_setvalue(number val) {
    this->gen_01_hb_lp_r_value = val;
}

void gen_01_hb_lp_r_reset() {
    this->gen_01_hb_lp_r_value = 0;
}

void gen_01_hb_lp_r_init() {
    this->gen_01_hb_lp_r_value = 0;
}

number gen_01_hb_lp2_r_getvalue() {
    return this->gen_01_hb_lp2_r_value;
}

void gen_01_hb_lp2_r_setvalue(number val) {
    this->gen_01_hb_lp2_r_value = val;
}

void gen_01_hb_lp2_r_reset() {
    this->gen_01_hb_lp2_r_value = 0;
}

void gen_01_hb_lp2_r_init() {
    this->gen_01_hb_lp2_r_value = 0;
}

number gen_01_press_low_l_getvalue() {
    return this->gen_01_press_low_l_value;
}

void gen_01_press_low_l_setvalue(number val) {
    this->gen_01_press_low_l_value = val;
}

void gen_01_press_low_l_reset() {
    this->gen_01_press_low_l_value = 0;
}

void gen_01_press_low_l_init() {
    this->gen_01_press_low_l_value = 0;
}

number gen_01_press_high_l_getvalue() {
    return this->gen_01_press_high_l_value;
}

void gen_01_press_high_l_setvalue(number val) {
    this->gen_01_press_high_l_value = val;
}

void gen_01_press_high_l_reset() {
    this->gen_01_press_high_l_value = 0;
}

void gen_01_press_high_l_init() {
    this->gen_01_press_high_l_value = 0;
}

number gen_01_press_low_r_getvalue() {
    return this->gen_01_press_low_r_value;
}

void gen_01_press_low_r_setvalue(number val) {
    this->gen_01_press_low_r_value = val;
}

void gen_01_press_low_r_reset() {
    this->gen_01_press_low_r_value = 0;
}

void gen_01_press_low_r_init() {
    this->gen_01_press_low_r_value = 0;
}

number gen_01_press_high_r_getvalue() {
    return this->gen_01_press_high_r_value;
}

void gen_01_press_high_r_setvalue(number val) {
    this->gen_01_press_high_r_value = val;
}

void gen_01_press_high_r_reset() {
    this->gen_01_press_high_r_value = 0;
}

void gen_01_press_high_r_init() {
    this->gen_01_press_high_r_value = 0;
}

void onepole_tilde_01_reset() {
    this->onepole_tilde_01_lastY = 0;
    this->onepole_tilde_01_a0 = 0;
    this->onepole_tilde_01_b1 = 0;
}

void onepole_tilde_01_dspsetup(bool force) {
    if ((bool)(this->onepole_tilde_01_setupDone) && (bool)(!(bool)(force)))
        return;

    this->onepole_tilde_01_needsUpdate = true;
    this->onepole_tilde_01_reset();
    this->onepole_tilde_01_setupDone = true;
}

number gen_02_ub_lp1_l_getvalue() {
    return this->gen_02_ub_lp1_l_value;
}

void gen_02_ub_lp1_l_setvalue(number val) {
    this->gen_02_ub_lp1_l_value = val;
}

void gen_02_ub_lp1_l_reset() {
    this->gen_02_ub_lp1_l_value = 0;
}

void gen_02_ub_lp1_l_init() {
    this->gen_02_ub_lp1_l_value = 0;
}

number gen_02_ub_lp2_l_getvalue() {
    return this->gen_02_ub_lp2_l_value;
}

void gen_02_ub_lp2_l_setvalue(number val) {
    this->gen_02_ub_lp2_l_value = val;
}

void gen_02_ub_lp2_l_reset() {
    this->gen_02_ub_lp2_l_value = 0;
}

void gen_02_ub_lp2_l_init() {
    this->gen_02_ub_lp2_l_value = 0;
}

number gen_02_ub_lp1_r_getvalue() {
    return this->gen_02_ub_lp1_r_value;
}

void gen_02_ub_lp1_r_setvalue(number val) {
    this->gen_02_ub_lp1_r_value = val;
}

void gen_02_ub_lp1_r_reset() {
    this->gen_02_ub_lp1_r_value = 0;
}

void gen_02_ub_lp1_r_init() {
    this->gen_02_ub_lp1_r_value = 0;
}

number gen_02_ub_lp2_r_getvalue() {
    return this->gen_02_ub_lp2_r_value;
}

void gen_02_ub_lp2_r_setvalue(number val) {
    this->gen_02_ub_lp2_r_value = val;
}

void gen_02_ub_lp2_r_reset() {
    this->gen_02_ub_lp2_r_value = 0;
}

void gen_02_ub_lp2_r_init() {
    this->gen_02_ub_lp2_r_value = 0;
}

number gen_03_x1_l_getvalue() {
    return this->gen_03_x1_l_value;
}

void gen_03_x1_l_setvalue(number val) {
    this->gen_03_x1_l_value = val;
}

void gen_03_x1_l_reset() {
    this->gen_03_x1_l_value = 0;
}

void gen_03_x1_l_init() {
    this->gen_03_x1_l_value = 0;
}

number gen_03_y1_l_getvalue() {
    return this->gen_03_y1_l_value;
}

void gen_03_y1_l_setvalue(number val) {
    this->gen_03_y1_l_value = val;
}

void gen_03_y1_l_reset() {
    this->gen_03_y1_l_value = 0;
}

void gen_03_y1_l_init() {
    this->gen_03_y1_l_value = 0;
}

number gen_03_x1_r_getvalue() {
    return this->gen_03_x1_r_value;
}

void gen_03_x1_r_setvalue(number val) {
    this->gen_03_x1_r_value = val;
}

void gen_03_x1_r_reset() {
    this->gen_03_x1_r_value = 0;
}

void gen_03_x1_r_init() {
    this->gen_03_x1_r_value = 0;
}

number gen_03_y1_r_getvalue() {
    return this->gen_03_y1_r_value;
}

void gen_03_y1_r_setvalue(number val) {
    this->gen_03_y1_r_value = val;
}

void gen_03_y1_r_reset() {
    this->gen_03_y1_r_value = 0;
}

void gen_03_y1_r_init() {
    this->gen_03_y1_r_value = 0;
}

number gen_03_lp_l_getvalue() {
    return this->gen_03_lp_l_value;
}

void gen_03_lp_l_setvalue(number val) {
    this->gen_03_lp_l_value = val;
}

void gen_03_lp_l_reset() {
    this->gen_03_lp_l_value = 0;
}

void gen_03_lp_l_init() {
    this->gen_03_lp_l_value = 0;
}

number gen_03_lp_r_getvalue() {
    return this->gen_03_lp_r_value;
}

void gen_03_lp_r_setvalue(number val) {
    this->gen_03_lp_r_value = val;
}

void gen_03_lp_r_reset() {
    this->gen_03_lp_r_value = 0;
}

void gen_03_lp_r_init() {
    this->gen_03_lp_r_value = 0;
}

void numberobj_07_init() {
    this->numberobj_07_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-73"), 1, this->_currentTime);
}

void numberobj_07_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_07_value;
}

void numberobj_07_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_07_value_set(preset["value"]);
}

void numberobj_08_init() {
    this->numberobj_08_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-86"), 1, this->_currentTime);
}

void numberobj_08_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_08_value;
}

void numberobj_08_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_08_value_set(preset["value"]);
}

void param_01_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_01_value;
}

void param_01_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_01_value_set(preset["value"]);
}

void onepole_tilde_02_reset() {
    this->onepole_tilde_02_lastY = 0;
    this->onepole_tilde_02_a0 = 0;
    this->onepole_tilde_02_b1 = 0;
}

void onepole_tilde_02_dspsetup(bool force) {
    if ((bool)(this->onepole_tilde_02_setupDone) && (bool)(!(bool)(force)))
        return;

    this->onepole_tilde_02_needsUpdate = true;
    this->onepole_tilde_02_reset();
    this->onepole_tilde_02_setupDone = true;
}

void numberobj_09_init() {
    this->numberobj_09_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-65"), 1, this->_currentTime);
}

void numberobj_09_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_09_value;
}

void numberobj_09_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_09_value_set(preset["value"]);
}

number numbertilde_03_smooth_d_next(number x) {
    number temp = (number)(x - this->numbertilde_03_smooth_d_prev);
    this->numbertilde_03_smooth_d_prev = x;
    return temp;
}

void numbertilde_03_smooth_d_dspsetup() {
    this->numbertilde_03_smooth_d_reset();
}

void numbertilde_03_smooth_d_reset() {
    this->numbertilde_03_smooth_d_prev = 0;
}

number numbertilde_03_smooth_next(number x, number up, number down) {
    if (this->numbertilde_03_smooth_d_next(x) != 0.) {
        if (x > this->numbertilde_03_smooth_prev) {
            number _up = up;

            if (_up < 1)
                _up = 1;

            this->numbertilde_03_smooth_index = _up;
            this->numbertilde_03_smooth_increment = (x - this->numbertilde_03_smooth_prev) / _up;
        } else if (x < this->numbertilde_03_smooth_prev) {
            number _down = down;

            if (_down < 1)
                _down = 1;

            this->numbertilde_03_smooth_index = _down;
            this->numbertilde_03_smooth_increment = (x - this->numbertilde_03_smooth_prev) / _down;
        }
    }

    if (this->numbertilde_03_smooth_index > 0) {
        this->numbertilde_03_smooth_prev += this->numbertilde_03_smooth_increment;
        this->numbertilde_03_smooth_index -= 1;
    } else {
        this->numbertilde_03_smooth_prev = x;
    }

    return this->numbertilde_03_smooth_prev;
}

void numbertilde_03_smooth_reset() {
    this->numbertilde_03_smooth_prev = 0;
    this->numbertilde_03_smooth_index = 0;
    this->numbertilde_03_smooth_increment = 0;
    this->numbertilde_03_smooth_d_reset();
}

void numbertilde_03_init() {
    this->numbertilde_03_currentMode = 1;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number~_obj-95"), 1, this->_currentTime);
}

void numbertilde_03_dspsetup(bool force) {
    if ((bool)(this->numbertilde_03_setupDone) && (bool)(!(bool)(force)))
        return;

    this->numbertilde_03_currentIntervalInSamples = this->mstosamps(100);
    this->numbertilde_03_currentInterval = this->numbertilde_03_currentIntervalInSamples;
    this->numbertilde_03_rampInSamples = this->mstosamps(this->numbertilde_03_ramp);
    this->numbertilde_03_setupDone = true;
    this->numbertilde_03_smooth_d_dspsetup();
}

void param_02_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_02_value;
}

void param_02_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_02_value_set(preset["value"]);
}

void numberobj_10_init() {
    this->numberobj_10_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-14"), 1, this->_currentTime);
}

void numberobj_10_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_10_value;
}

void numberobj_10_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_10_value_set(preset["value"]);
}

void param_03_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_03_value;
}

void param_03_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_03_value_set(preset["value"]);
}

void param_04_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_04_value;
}

void param_04_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_04_value_set(preset["value"]);
}

void ip_01_init() {
    this->ip_01_lastValue = this->ip_01_value;
}

void ip_01_dspsetup(bool force) {
    if ((bool)(this->ip_01_setupDone) && (bool)(!(bool)(force)))
        return;

    this->ip_01_lastIndex = 0;
    this->ip_01_setupDone = true;
}

void param_05_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_05_value;
}

void param_05_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_05_value_set(preset["value"]);
}

void numberobj_11_init() {
    this->numberobj_11_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-27"), 1, this->_currentTime);
}

void numberobj_11_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_11_value;
}

void numberobj_11_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_11_value_set(preset["value"]);
}

void param_06_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_06_value;
}

void param_06_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_06_value_set(preset["value"]);
}

void numberobj_12_init() {
    this->numberobj_12_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-99"), 1, this->_currentTime);
}

void numberobj_12_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_12_value;
}

void numberobj_12_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_12_value_set(preset["value"]);
}

void param_07_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_07_value;
}

void param_07_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_07_value_set(preset["value"]);
}

void numberobj_13_init() {
    this->numberobj_13_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-167"), 1, this->_currentTime);
}

void numberobj_13_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_13_value;
}

void numberobj_13_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_13_value_set(preset["value"]);
}

void numberobj_14_init() {
    this->numberobj_14_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-48"), 1, this->_currentTime);
}

void numberobj_14_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_14_value;
}

void numberobj_14_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_14_value_set(preset["value"]);
}

void param_08_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_08_value;
}

void param_08_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_08_value_set(preset["value"]);
}

void numberobj_15_init() {
    this->numberobj_15_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-67"), 1, this->_currentTime);
}

void numberobj_15_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_15_value;
}

void numberobj_15_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_15_value_set(preset["value"]);
}

void numberobj_16_init() {
    this->numberobj_16_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-22"), 1, this->_currentTime);
}

void numberobj_16_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_16_value;
}

void numberobj_16_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_16_value_set(preset["value"]);
}

void numberobj_17_init() {
    this->numberobj_17_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-78"), 1, this->_currentTime);
}

void numberobj_17_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_17_value;
}

void numberobj_17_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_17_value_set(preset["value"]);
}

void numberobj_18_init() {
    this->numberobj_18_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-102"), 1, this->_currentTime);
}

void numberobj_18_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_18_value;
}

void numberobj_18_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_18_value_set(preset["value"]);
}

void numberobj_19_init() {
    this->numberobj_19_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-125"), 1, this->_currentTime);
}

void numberobj_19_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_19_value;
}

void numberobj_19_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_19_value_set(preset["value"]);
}

void numberobj_20_init() {
    this->numberobj_20_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-129"), 1, this->_currentTime);
}

void numberobj_20_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_20_value;
}

void numberobj_20_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_20_value_set(preset["value"]);
}

void numberobj_21_init() {
    this->numberobj_21_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-134"), 1, this->_currentTime);
}

void numberobj_21_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_21_value;
}

void numberobj_21_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_21_value_set(preset["value"]);
}

void numberobj_22_init() {
    this->numberobj_22_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-136"), 1, this->_currentTime);
}

void numberobj_22_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_22_value;
}

void numberobj_22_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_22_value_set(preset["value"]);
}

void numberobj_23_init() {
    this->numberobj_23_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-138"), 1, this->_currentTime);
}

void numberobj_23_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_23_value;
}

void numberobj_23_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_23_value_set(preset["value"]);
}

void numberobj_24_init() {
    this->numberobj_24_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-141"), 1, this->_currentTime);
}

void numberobj_24_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_24_value;
}

void numberobj_24_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_24_value_set(preset["value"]);
}

void numberobj_25_init() {
    this->numberobj_25_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-149"), 1, this->_currentTime);
}

void numberobj_25_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_25_value;
}

void numberobj_25_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_25_value_set(preset["value"]);
}

void numberobj_26_init() {
    this->numberobj_26_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-155"), 1, this->_currentTime);
}

void numberobj_26_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_26_value;
}

void numberobj_26_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_26_value_set(preset["value"]);
}

void numberobj_27_init() {
    this->numberobj_27_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-172"), 1, this->_currentTime);
}

void numberobj_27_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_27_value;
}

void numberobj_27_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_27_value_set(preset["value"]);
}

void numberobj_28_init() {
    this->numberobj_28_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-158"), 1, this->_currentTime);
}

void numberobj_28_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_28_value;
}

void numberobj_28_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_28_value_set(preset["value"]);
}

void numberobj_29_init() {
    this->numberobj_29_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-179"), 1, this->_currentTime);
}

void numberobj_29_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_29_value;
}

void numberobj_29_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_29_value_set(preset["value"]);
}

void globaltransport_advance() {}

void globaltransport_dspsetup(bool ) {}

bool stackprotect_check() {
    this->stackprotect_count++;

    if (this->stackprotect_count > 128) {
        console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
        return true;
    }

    return false;
}

void updateTime(MillisecondTime time) {
    this->_currentTime = time;
    this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));

    if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;

    if (this->sampleOffsetIntoNextAudioBuffer < 0)
        this->sampleOffsetIntoNextAudioBuffer = 0;
}

void assign_defaults()
{
    dspexpr_01_in1 = 0;
    dspexpr_01_in2 = 0;
    dspexpr_01_in3 = 0;
    dspexpr_02_in1 = 0;
    dspexpr_02_in2 = 0;
    dspexpr_02_in3 = 0;
    dspexpr_03_in1 = 0;
    dspexpr_03_in2 = 1;
    gen_01_in1 = 0;
    gen_01_in2 = 0;
    gen_01_in3 = 0;
    gen_01_in4 = 0;
    onepole_tilde_01_x = 0;
    onepole_tilde_01_freqInHz = 13000;
    dspexpr_04_in1 = 0;
    dspexpr_04_in2 = 1;
    dspexpr_05_in1 = 0;
    dspexpr_05_in2 = 1;
    dspexpr_06_in1 = 0;
    dspexpr_06_in2 = 1;
    p_01_target = 0;
    p_02_target = 0;
    gen_02_in1 = 0;
    gen_02_in2 = 0;
    gen_02_in3 = 0;
    gen_03_in1 = 0;
    gen_03_in2 = 0;
    gen_03_in3 = 0;
    gen_03_in4 = 0;
    gen_03_in5 = 0;
    gen_03_in6 = 0;
    gen_03_in7 = 0;
    gen_03_in8 = 0;
    gen_03_in9 = 0;
    gen_03_in10 = 0;
    dspexpr_07_in1 = 0;
    dspexpr_07_in2 = 1;
    expr_05_in1 = 0;
    expr_05_in2 = 0.5;
    expr_05_out1 = 0;
    numberobj_07_value = 0;
    numberobj_07_value_setter(numberobj_07_value);
    linetilde_05_time = 10;
    linetilde_05_keepramp = 1;
    numberobj_08_value = 0;
    numberobj_08_value_setter(numberobj_08_value);
    expr_06_in1 = 0;
    expr_06_out1 = 0;
    expr_07_in1 = 0;
    expr_07_out1 = 0;
    linetilde_06_time = 10;
    linetilde_06_keepramp = 1;
    param_01_value = 0;
    onepole_tilde_02_x = 0;
    onepole_tilde_02_freqInHz = 13000;
    dspexpr_08_in1 = 0;
    dspexpr_08_in2 = 1;
    dspexpr_09_in1 = 0;
    dspexpr_09_in2 = 1;
    dspexpr_10_in1 = 0;
    dspexpr_10_in2 = 1;
    linetilde_07_time = 10;
    linetilde_07_keepramp = 1;
    numberobj_09_value = 0;
    numberobj_09_value_setter(numberobj_09_value);
    expr_08_in1 = 0;
    expr_08_out1 = 0;
    expr_09_in1 = 0;
    expr_09_out1 = 0;
    numbertilde_03_input_number = 0;
    numbertilde_03_ramp = 0;
    linetilde_08_time = 10;
    linetilde_08_keepramp = 1;
    param_02_value = 0;
    numberobj_10_value = 0;
    numberobj_10_value_setter(numberobj_10_value);
    param_03_value = 1.16;
    dspexpr_11_in1 = 0;
    dspexpr_11_in2 = 0;
    dspexpr_11_in3 = 0;
    linetilde_09_time = 10;
    linetilde_09_keepramp = 1;
    param_04_value = 1;
    ip_01_value = 0;
    ip_01_impulse = 0;
    linetilde_10_time = 10;
    linetilde_10_keepramp = 1;
    param_05_value = 0;
    expr_10_in1 = 0;
    expr_10_in2 = 0;
    expr_10_in3 = 1;
    expr_10_out1 = 0;
    scale_01_inlow = -100;
    scale_01_inhigh = 6;
    scale_01_outlow = 0;
    scale_01_outhigh = 1;
    scale_01_power = 1;
    snapshot_01_interval = 0;
    snapshot_01_out = 0;
    dspexpr_12_in1 = 0;
    dspexpr_12_in2 = 0.5;
    dspexpr_13_in1 = 0;
    dspexpr_13_in2 = 0;
    dspexpr_14_in1 = 0;
    slide_tilde_01_x = 0;
    slide_tilde_01_up = 50;
    slide_tilde_01_down = 2000;
    dspexpr_15_in1 = 0;
    numberobj_11_value = 0;
    numberobj_11_value_setter(numberobj_11_value);
    expr_11_in1 = 0;
    expr_11_in2 = 0;
    expr_11_out1 = 0;
    expr_12_in1 = 0;
    expr_12_in2 = 0;
    expr_12_out1 = 0;
    param_06_value = 0;
    numberobj_12_value = 0;
    numberobj_12_value_setter(numberobj_12_value);
    param_07_value = 0;
    numberobj_13_value = 0;
    numberobj_13_value_setter(numberobj_13_value);
    linetilde_11_time = 10;
    linetilde_11_keepramp = 1;
    numberobj_14_value = 0;
    numberobj_14_value_setter(numberobj_14_value);
    codebox_01_in1 = 0;
    codebox_01_out1 = 0;
    codebox_01_out2 = 0;
    codebox_01_out3 = 0;
    codebox_01_out4 = 0;
    codebox_01_out5 = 0;
    codebox_01_out6 = 0;
    codebox_01_out7 = 0;
    codebox_01_out8 = 0;
    codebox_01_out9 = 0;
    codebox_01_out10 = 0;
    codebox_01_out11 = 0;
    codebox_01_out12 = 0;
    codebox_01_out13 = 0;
    codebox_01_out14 = 0;
    codebox_01_out15 = 0;
    param_08_value = 0;
    linetilde_12_time = 10;
    linetilde_12_keepramp = 1;
    numberobj_15_value = 0;
    numberobj_15_value_setter(numberobj_15_value);
    linetilde_13_time = 10;
    linetilde_13_keepramp = 1;
    numberobj_16_value = 0;
    numberobj_16_value_setter(numberobj_16_value);
    numberobj_17_value = 0;
    numberobj_17_value_setter(numberobj_17_value);
    linetilde_14_time = 10;
    linetilde_14_keepramp = 1;
    numberobj_18_value = 0;
    numberobj_18_value_setter(numberobj_18_value);
    linetilde_15_time = 10;
    linetilde_15_keepramp = 1;
    numberobj_19_value = 0;
    numberobj_19_value_setter(numberobj_19_value);
    linetilde_16_time = 10;
    linetilde_16_keepramp = 1;
    numberobj_20_value = 0;
    numberobj_20_value_setter(numberobj_20_value);
    linetilde_17_time = 10;
    linetilde_17_keepramp = 1;
    numberobj_21_value = 0;
    numberobj_21_value_setter(numberobj_21_value);
    linetilde_18_time = 10;
    linetilde_18_keepramp = 1;
    numberobj_22_value = 0;
    numberobj_22_value_setter(numberobj_22_value);
    numberobj_23_value = 0;
    numberobj_23_value_setter(numberobj_23_value);
    linetilde_19_time = 10;
    linetilde_19_keepramp = 1;
    numberobj_24_value = 0;
    numberobj_24_value_setter(numberobj_24_value);
    linetilde_20_time = 10;
    linetilde_20_keepramp = 1;
    numberobj_25_value = 0;
    numberobj_25_value_setter(numberobj_25_value);
    linetilde_21_time = 10;
    linetilde_21_keepramp = 1;
    numberobj_26_value = 0;
    numberobj_26_value_setter(numberobj_26_value);
    numberobj_27_value = 0;
    numberobj_27_value_setter(numberobj_27_value);
    numberobj_28_value = 0;
    numberobj_28_value_setter(numberobj_28_value);
    numberobj_29_value = 0;
    numberobj_29_value_setter(numberobj_29_value);
    _currentTime = 0;
    audioProcessSampleCount = 0;
    sampleOffsetIntoNextAudioBuffer = 0;
    zeroBuffer = nullptr;
    dummyBuffer = nullptr;
    signals[0] = nullptr;
    signals[1] = nullptr;
    signals[2] = nullptr;
    signals[3] = nullptr;
    signals[4] = nullptr;
    signals[5] = nullptr;
    signals[6] = nullptr;
    signals[7] = nullptr;
    signals[8] = nullptr;
    signals[9] = nullptr;
    signals[10] = nullptr;
    signals[11] = nullptr;
    signals[12] = nullptr;
    signals[13] = nullptr;
    signals[14] = nullptr;
    signals[15] = nullptr;
    didAllocateSignals = 0;
    vs = 0;
    maxvs = 0;
    sr = 44100;
    invsr = 0.00002267573696;
    gen_01_hb_lp_l_value = 0;
    gen_01_hb_lp2_l_value = 0;
    gen_01_hb_lp_r_value = 0;
    gen_01_hb_lp2_r_value = 0;
    gen_01_press_low_l_value = 0;
    gen_01_press_high_l_value = 0;
    gen_01_press_low_r_value = 0;
    gen_01_press_high_r_value = 0;
    onepole_tilde_01_freq = 0;
    onepole_tilde_01_needsUpdate = false;
    onepole_tilde_01_lastY = 0;
    onepole_tilde_01_a0 = 0;
    onepole_tilde_01_b1 = 0;
    onepole_tilde_01_setupDone = false;
    gen_02_ub_lp1_l_value = 0;
    gen_02_ub_lp2_l_value = 0;
    gen_02_ub_lp1_r_value = 0;
    gen_02_ub_lp2_r_value = 0;
    gen_03_x1_l_value = 0;
    gen_03_y1_l_value = 0;
    gen_03_x1_r_value = 0;
    gen_03_y1_r_value = 0;
    gen_03_lp_l_value = 0;
    gen_03_lp_r_value = 0;
    numberobj_07_currentFormat = 6;
    numberobj_07_lastValue = 0;
    linetilde_05_currentValue = 1;
    numberobj_08_currentFormat = 6;
    numberobj_08_lastValue = 0;
    linetilde_06_currentValue = 1;
    param_01_lastValue = 0;
    onepole_tilde_02_freq = 0;
    onepole_tilde_02_needsUpdate = false;
    onepole_tilde_02_lastY = 0;
    onepole_tilde_02_a0 = 0;
    onepole_tilde_02_b1 = 0;
    onepole_tilde_02_setupDone = false;
    linetilde_07_currentValue = 1;
    numberobj_09_currentFormat = 6;
    numberobj_09_lastValue = 0;
    numbertilde_03_currentInterval = 0;
    numbertilde_03_currentIntervalInSamples = 0;
    numbertilde_03_lastValue = 0;
    numbertilde_03_outValue = 0;
    numbertilde_03_rampInSamples = 0;
    numbertilde_03_currentMode = 0;
    numbertilde_03_smooth_d_prev = 0;
    numbertilde_03_smooth_prev = 0;
    numbertilde_03_smooth_index = 0;
    numbertilde_03_smooth_increment = 0;
    numbertilde_03_setupDone = false;
    linetilde_08_currentValue = 1;
    param_02_lastValue = 0;
    numberobj_10_currentFormat = 6;
    numberobj_10_lastValue = 0;
    param_03_lastValue = 0;
    linetilde_09_currentValue = 1;
    param_04_lastValue = 0;
    ip_01_lastIndex = 0;
    ip_01_lastValue = 0;
    ip_01_resetCount = 0;
    ip_01_sigbuf = nullptr;
    ip_01_setupDone = false;
    linetilde_10_currentValue = 1;
    param_05_lastValue = 0;
    snapshot_01_calc = 0;
    snapshot_01_nextTime = 0;
    snapshot_01_count = 0;
    snapshot_01_lastValue = 0;
    slide_tilde_01_prev = 0;
    numberobj_11_currentFormat = 6;
    numberobj_11_lastValue = 0;
    param_06_lastValue = 0;
    numberobj_12_currentFormat = 6;
    numberobj_12_lastValue = 0;
    param_07_lastValue = 0;
    numberobj_13_currentFormat = 6;
    numberobj_13_lastValue = 0;
    linetilde_11_currentValue = 1;
    numberobj_14_currentFormat = 6;
    numberobj_14_lastValue = 0;
    param_08_lastValue = 0;
    linetilde_12_currentValue = 1;
    numberobj_15_currentFormat = 6;
    numberobj_15_lastValue = 0;
    linetilde_13_currentValue = 1;
    numberobj_16_currentFormat = 6;
    numberobj_16_lastValue = 0;
    numberobj_17_currentFormat = 6;
    numberobj_17_lastValue = 0;
    linetilde_14_currentValue = 1;
    numberobj_18_currentFormat = 6;
    numberobj_18_lastValue = 0;
    linetilde_15_currentValue = 1;
    numberobj_19_currentFormat = 6;
    numberobj_19_lastValue = 0;
    linetilde_16_currentValue = 1;
    numberobj_20_currentFormat = 6;
    numberobj_20_lastValue = 0;
    linetilde_17_currentValue = 1;
    numberobj_21_currentFormat = 6;
    numberobj_21_lastValue = 0;
    linetilde_18_currentValue = 1;
    numberobj_22_currentFormat = 6;
    numberobj_22_lastValue = 0;
    numberobj_23_currentFormat = 6;
    numberobj_23_lastValue = 0;
    linetilde_19_currentValue = 1;
    numberobj_24_currentFormat = 6;
    numberobj_24_lastValue = 0;
    linetilde_20_currentValue = 1;
    numberobj_25_currentFormat = 6;
    numberobj_25_lastValue = 0;
    linetilde_21_currentValue = 1;
    numberobj_26_currentFormat = 6;
    numberobj_26_lastValue = 0;
    numberobj_27_currentFormat = 6;
    numberobj_27_lastValue = 0;
    numberobj_28_currentFormat = 6;
    numberobj_28_lastValue = 0;
    numberobj_29_currentFormat = 6;
    numberobj_29_lastValue = 0;
    globaltransport_tempo = nullptr;
    globaltransport_state = nullptr;
    stackprotect_count = 0;
    _voiceIndex = 0;
    _noteNumber = 0;
    isMuted = 1;
}

// member variables

    number dspexpr_01_in1;
    number dspexpr_01_in2;
    number dspexpr_01_in3;
    number dspexpr_02_in1;
    number dspexpr_02_in2;
    number dspexpr_02_in3;
    number dspexpr_03_in1;
    number dspexpr_03_in2;
    number gen_01_in1;
    number gen_01_in2;
    number gen_01_in3;
    number gen_01_in4;
    number onepole_tilde_01_x;
    number onepole_tilde_01_freqInHz;
    number dspexpr_04_in1;
    number dspexpr_04_in2;
    number dspexpr_05_in1;
    number dspexpr_05_in2;
    number dspexpr_06_in1;
    number dspexpr_06_in2;
    number p_01_target;
    number p_02_target;
    number gen_02_in1;
    number gen_02_in2;
    number gen_02_in3;
    number gen_03_in1;
    number gen_03_in2;
    number gen_03_in3;
    number gen_03_in4;
    number gen_03_in5;
    number gen_03_in6;
    number gen_03_in7;
    number gen_03_in8;
    number gen_03_in9;
    number gen_03_in10;
    number dspexpr_07_in1;
    number dspexpr_07_in2;
    number expr_05_in1;
    number expr_05_in2;
    number expr_05_out1;
    number numberobj_07_value;
    list linetilde_05_segments;
    number linetilde_05_time;
    number linetilde_05_keepramp;
    number numberobj_08_value;
    number expr_06_in1;
    number expr_06_out1;
    number expr_07_in1;
    number expr_07_out1;
    list linetilde_06_segments;
    number linetilde_06_time;
    number linetilde_06_keepramp;
    number param_01_value;
    number onepole_tilde_02_x;
    number onepole_tilde_02_freqInHz;
    number dspexpr_08_in1;
    number dspexpr_08_in2;
    number dspexpr_09_in1;
    number dspexpr_09_in2;
    number dspexpr_10_in1;
    number dspexpr_10_in2;
    list linetilde_07_segments;
    number linetilde_07_time;
    number linetilde_07_keepramp;
    number numberobj_09_value;
    number expr_08_in1;
    number expr_08_out1;
    number expr_09_in1;
    number expr_09_out1;
    number numbertilde_03_input_number;
    number numbertilde_03_ramp;
    list linetilde_08_segments;
    number linetilde_08_time;
    number linetilde_08_keepramp;
    number param_02_value;
    number numberobj_10_value;
    number param_03_value;
    number dspexpr_11_in1;
    number dspexpr_11_in2;
    number dspexpr_11_in3;
    list linetilde_09_segments;
    number linetilde_09_time;
    number linetilde_09_keepramp;
    number param_04_value;
    number ip_01_value;
    number ip_01_impulse;
    list linetilde_10_segments;
    number linetilde_10_time;
    number linetilde_10_keepramp;
    number param_05_value;
    number expr_10_in1;
    number expr_10_in2;
    number expr_10_in3;
    number expr_10_out1;
    list scale_01_input;
    number scale_01_inlow;
    number scale_01_inhigh;
    number scale_01_outlow;
    number scale_01_outhigh;
    number scale_01_power;
    list scale_01_out;
    number snapshot_01_interval;
    number snapshot_01_out;
    number dspexpr_12_in1;
    number dspexpr_12_in2;
    number dspexpr_13_in1;
    number dspexpr_13_in2;
    number dspexpr_14_in1;
    number slide_tilde_01_x;
    number slide_tilde_01_up;
    number slide_tilde_01_down;
    number dspexpr_15_in1;
    number numberobj_11_value;
    number expr_11_in1;
    number expr_11_in2;
    number expr_11_out1;
    number expr_12_in1;
    number expr_12_in2;
    number expr_12_out1;
    number param_06_value;
    number numberobj_12_value;
    number param_07_value;
    number numberobj_13_value;
    list linetilde_11_segments;
    number linetilde_11_time;
    number linetilde_11_keepramp;
    number numberobj_14_value;
    number codebox_01_in1;
    number codebox_01_out1;
    number codebox_01_out2;
    number codebox_01_out3;
    number codebox_01_out4;
    number codebox_01_out5;
    number codebox_01_out6;
    number codebox_01_out7;
    number codebox_01_out8;
    number codebox_01_out9;
    number codebox_01_out10;
    number codebox_01_out11;
    number codebox_01_out12;
    number codebox_01_out13;
    number codebox_01_out14;
    number codebox_01_out15;
    number param_08_value;
    list linetilde_12_segments;
    number linetilde_12_time;
    number linetilde_12_keepramp;
    number numberobj_15_value;
    list linetilde_13_segments;
    number linetilde_13_time;
    number linetilde_13_keepramp;
    number numberobj_16_value;
    number numberobj_17_value;
    list linetilde_14_segments;
    number linetilde_14_time;
    number linetilde_14_keepramp;
    number numberobj_18_value;
    list linetilde_15_segments;
    number linetilde_15_time;
    number linetilde_15_keepramp;
    number numberobj_19_value;
    list linetilde_16_segments;
    number linetilde_16_time;
    number linetilde_16_keepramp;
    number numberobj_20_value;
    list linetilde_17_segments;
    number linetilde_17_time;
    number linetilde_17_keepramp;
    number numberobj_21_value;
    list linetilde_18_segments;
    number linetilde_18_time;
    number linetilde_18_keepramp;
    number numberobj_22_value;
    number numberobj_23_value;
    list linetilde_19_segments;
    number linetilde_19_time;
    number linetilde_19_keepramp;
    number numberobj_24_value;
    list linetilde_20_segments;
    number linetilde_20_time;
    number linetilde_20_keepramp;
    number numberobj_25_value;
    list linetilde_21_segments;
    number linetilde_21_time;
    number linetilde_21_keepramp;
    number numberobj_26_value;
    number numberobj_27_value;
    number numberobj_28_value;
    number numberobj_29_value;
    MillisecondTime _currentTime;
    UInt64 audioProcessSampleCount;
    SampleIndex sampleOffsetIntoNextAudioBuffer;
    signal zeroBuffer;
    signal dummyBuffer;
    SampleValue * signals[16];
    bool didAllocateSignals;
    Index vs;
    Index maxvs;
    number sr;
    number invsr;
    number gen_01_hb_lp_l_value;
    number gen_01_hb_lp2_l_value;
    number gen_01_hb_lp_r_value;
    number gen_01_hb_lp2_r_value;
    number gen_01_press_low_l_value;
    number gen_01_press_high_l_value;
    number gen_01_press_low_r_value;
    number gen_01_press_high_r_value;
    number onepole_tilde_01_freq;
    bool onepole_tilde_01_needsUpdate;
    number onepole_tilde_01_lastY;
    number onepole_tilde_01_a0;
    number onepole_tilde_01_b1;
    bool onepole_tilde_01_setupDone;
    number gen_02_ub_lp1_l_value;
    number gen_02_ub_lp2_l_value;
    number gen_02_ub_lp1_r_value;
    number gen_02_ub_lp2_r_value;
    number gen_03_x1_l_value;
    number gen_03_y1_l_value;
    number gen_03_x1_r_value;
    number gen_03_y1_r_value;
    number gen_03_lp_l_value;
    number gen_03_lp_r_value;
    Int numberobj_07_currentFormat;
    number numberobj_07_lastValue;
    list linetilde_05_activeRamps;
    number linetilde_05_currentValue;
    Int numberobj_08_currentFormat;
    number numberobj_08_lastValue;
    list linetilde_06_activeRamps;
    number linetilde_06_currentValue;
    number param_01_lastValue;
    number onepole_tilde_02_freq;
    bool onepole_tilde_02_needsUpdate;
    number onepole_tilde_02_lastY;
    number onepole_tilde_02_a0;
    number onepole_tilde_02_b1;
    bool onepole_tilde_02_setupDone;
    list linetilde_07_activeRamps;
    number linetilde_07_currentValue;
    Int numberobj_09_currentFormat;
    number numberobj_09_lastValue;
    SampleIndex numbertilde_03_currentInterval;
    SampleIndex numbertilde_03_currentIntervalInSamples;
    number numbertilde_03_lastValue;
    number numbertilde_03_outValue;
    number numbertilde_03_rampInSamples;
    Int numbertilde_03_currentMode;
    number numbertilde_03_smooth_d_prev;
    number numbertilde_03_smooth_prev;
    number numbertilde_03_smooth_index;
    number numbertilde_03_smooth_increment;
    bool numbertilde_03_setupDone;
    list linetilde_08_activeRamps;
    number linetilde_08_currentValue;
    number param_02_lastValue;
    Int numberobj_10_currentFormat;
    number numberobj_10_lastValue;
    number param_03_lastValue;
    list linetilde_09_activeRamps;
    number linetilde_09_currentValue;
    number param_04_lastValue;
    SampleIndex ip_01_lastIndex;
    number ip_01_lastValue;
    SampleIndex ip_01_resetCount;
    signal ip_01_sigbuf;
    bool ip_01_setupDone;
    list linetilde_10_activeRamps;
    number linetilde_10_currentValue;
    number param_05_lastValue;
    number snapshot_01_calc;
    number snapshot_01_nextTime;
    SampleIndex snapshot_01_count;
    number snapshot_01_lastValue;
    number slide_tilde_01_prev;
    Int numberobj_11_currentFormat;
    number numberobj_11_lastValue;
    number param_06_lastValue;
    Int numberobj_12_currentFormat;
    number numberobj_12_lastValue;
    number param_07_lastValue;
    Int numberobj_13_currentFormat;
    number numberobj_13_lastValue;
    list linetilde_11_activeRamps;
    number linetilde_11_currentValue;
    Int numberobj_14_currentFormat;
    number numberobj_14_lastValue;
    number param_08_lastValue;
    list linetilde_12_activeRamps;
    number linetilde_12_currentValue;
    Int numberobj_15_currentFormat;
    number numberobj_15_lastValue;
    list linetilde_13_activeRamps;
    number linetilde_13_currentValue;
    Int numberobj_16_currentFormat;
    number numberobj_16_lastValue;
    Int numberobj_17_currentFormat;
    number numberobj_17_lastValue;
    list linetilde_14_activeRamps;
    number linetilde_14_currentValue;
    Int numberobj_18_currentFormat;
    number numberobj_18_lastValue;
    list linetilde_15_activeRamps;
    number linetilde_15_currentValue;
    Int numberobj_19_currentFormat;
    number numberobj_19_lastValue;
    list linetilde_16_activeRamps;
    number linetilde_16_currentValue;
    Int numberobj_20_currentFormat;
    number numberobj_20_lastValue;
    list linetilde_17_activeRamps;
    number linetilde_17_currentValue;
    Int numberobj_21_currentFormat;
    number numberobj_21_lastValue;
    list linetilde_18_activeRamps;
    number linetilde_18_currentValue;
    Int numberobj_22_currentFormat;
    number numberobj_22_lastValue;
    Int numberobj_23_currentFormat;
    number numberobj_23_lastValue;
    list linetilde_19_activeRamps;
    number linetilde_19_currentValue;
    Int numberobj_24_currentFormat;
    number numberobj_24_lastValue;
    list linetilde_20_activeRamps;
    number linetilde_20_currentValue;
    Int numberobj_25_currentFormat;
    number numberobj_25_lastValue;
    list linetilde_21_activeRamps;
    number linetilde_21_currentValue;
    Int numberobj_26_currentFormat;
    number numberobj_26_lastValue;
    Int numberobj_27_currentFormat;
    number numberobj_27_lastValue;
    Int numberobj_28_currentFormat;
    number numberobj_28_lastValue;
    Int numberobj_29_currentFormat;
    number numberobj_29_lastValue;
    signal globaltransport_tempo;
    signal globaltransport_state;
    number stackprotect_count;
    Index _voiceIndex;
    Int _noteNumber;
    Index isMuted;
    indexlist paramInitIndices;
    indexlist paramInitOrder;
    RNBOSubpatcher_05* p_01;
    RNBOSubpatcher_06* p_02;

};

PatcherInterface* createKanal42110()
{
    return new Kanal42110();
}

#ifndef RNBO_NO_PATCHERFACTORY

extern "C" PatcherFactoryFunctionPtr GetPatcherFactoryFunction(PlatformInterface* platformInterface)
#else

extern "C" PatcherFactoryFunctionPtr Kanal42110FactoryFunction(PlatformInterface* platformInterface)
#endif

{
    Platform::set(platformInterface);
    return createKanal42110;
}

} // end RNBO namespace

